#!/usr/bin/env python3

#数据筛选器
#python filter.py data.json

"""
Read MQTTX-exported JSON or JSONL of payloads, extract time_ms/depth_m samples,
then select samples where |depth - TARGET_DEPTH_DEEP| < MAX_DEPTH_OFFSET
or |depth - TARGET_DEPTH_SHALLOW| < MAX_DEPTH_OFFSET.

Usage:
  python tools/filter_depth_targets.py input.json --config ../Config.h --output matches.jsonl

Prints matched records in time order and optionally writes JSONL.
"""
import argparse
import json
import re
from pathlib import Path
from typing import List, Dict, Any

CONFIG_KEYS = ("TARGET_DEPTH_DEEP", "TARGET_DEPTH_SHALLOW", "MAX_DEPTH_OFFSET")


def parse_config(header_path: Path):
    vals = {}
    if not header_path.exists():
        raise FileNotFoundError(f"config header not found: {header_path}")
    text = header_path.read_text(encoding="utf-8")
    for key in CONFIG_KEYS:
        m = re.search(rf"#define\s+{key}\s+([-+]?[0-9]*\.?[0-9]+)f?", text)
        if m:
            vals[key] = float(m.group(1))
    if len(vals) != len(CONFIG_KEYS):
        missing = set(CONFIG_KEYS) - set(vals.keys())
        raise ValueError(f"missing defines in config: {missing}")
    return vals


def extract_payload_obj(item: Dict[str, Any]):
    # MQTTX export: item may contain 'payload' as JSON-encoded string
    if not isinstance(item, dict):
        return None
    if "payload" in item:
        payload = item["payload"]
        if isinstance(payload, str):
            try:
                return json.loads(payload)
            except json.JSONDecodeError:
                return None
        if isinstance(payload, dict):
            return payload
    # if item itself is a sample-like dict
    return item


def collect_records_from_loaded(obj) -> List[Dict[str, Any]]:
    records = []
    if isinstance(obj, list):
        for entry in obj:
            # MQTTX top-level connection object contains 'messages' list
            if isinstance(entry, dict) and "messages" in entry and isinstance(entry["messages"], list):
                for m in entry["messages"]:
                    p = extract_payload_obj(m)
                    if p:
                        records.append(p)
            else:
                p = extract_payload_obj(entry)
                if p:
                    records.append(p)
        return records
    if isinstance(obj, dict):
        # direct messages container
        if "messages" in obj and isinstance(obj["messages"], list):
            for m in obj["messages"]:
                p = extract_payload_obj(m)
                if p:
                    records.append(p)
            return records
        # maybe the file is a single payload
        p = extract_payload_obj(obj)
        if p:
            records.append(p)
        return records
    return records


def normalize(record: Dict[str, Any]):
    # try multiple keys
    time_keys = ("time_ms", "time", "timestamp_ms", "timeMs", "timestamp")
    depth_keys = ("depth_m", "depth", "depthMeter", "depth_meters", "depthMeters")
    out = {}
    for k in time_keys:
        if k in record:
            out["time_ms"] = float(record[k])
            break
    for k in depth_keys:
        if k in record:
            out["depth_m"] = float(record[k])
            break
    if "time_ms" in out and "depth_m" in out:
        return out
    return None


def load_input(path: Path) -> List[Dict[str, Any]]:
    text = path.read_text(encoding="utf-8").strip()
    if not text:
        return []
    # try full JSON parse
    try:
        obj = json.loads(text)
        records = collect_records_from_loaded(obj)
    except json.JSONDecodeError:
        # fallback: parse line-by-line JSON (JSONL)
        records = []
        for line in text.splitlines():
            line = line.strip()
            if not line:
                continue
            try:
                obj = json.loads(line)
            except json.JSONDecodeError:
                continue
            records.extend(collect_records_from_loaded(obj))
    # normalize
    normalized = []
    for r in records:
        n = normalize(r)
        if n:
            normalized.append(n)
    return normalized


def filter_targets(records: List[Dict[str, Any]], cfg: Dict[str, float]):
    deep = cfg["TARGET_DEPTH_DEEP"]
    shallow = cfg["TARGET_DEPTH_SHALLOW"]
    tol = cfg["MAX_DEPTH_OFFSET"]
    matched = []
    for r in records:
        d = r["depth_m"]
        if abs(d - deep) <= tol or abs(d - shallow) <= tol:
            # annotate which target matched and deviation
            dev_deep = abs(d - deep)
            dev_shallow = abs(d - shallow)
            target = "deep" if dev_deep <= tol else "shallow"
            deviation = dev_deep if target == "deep" else dev_shallow
            matched.append({
                "time_ms": r["time_ms"],
                "depth_m": d,
                "target": target,
                "deviation": deviation,
            })
    matched.sort(key=lambda x: x["time_ms"])
    return matched


def main():
    p = argparse.ArgumentParser()
    p.add_argument("input", help="input JSON/JSONL or MQTTX-export file")
    p.add_argument("--config", default="../Config.h", help="path to Config.h to read target constants")
    p.add_argument("--output", help="optional output JSONL file to save matched records")
    args = p.parse_args()

    input_path = Path(args.input)
    cfg_path = Path(args.config)

    cfg = parse_config(cfg_path)
    records = load_input(input_path)
    if not records:
        print("no valid records found")
        return
    matched = filter_targets(records, cfg)
    for m in matched:
        t = m["time_ms"]
        d = m["depth_m"]
        print(f"{t:>10} ms  depth={d:6.2f}  target={m['target']}  dev={m['deviation']:.3f}")

    if args.output:
        outp = Path(args.output)
        with outp.open("w", encoding="utf-8") as fh:
            for m in matched:
                fh.write(json.dumps(m, ensure_ascii=False) + "\n")
        print(f"wrote {len(matched)} records to {outp}")

if __name__ == "__main__":
    main()
