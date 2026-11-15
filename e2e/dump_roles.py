#!/usr/bin/env python3
"""Print every occurrence in a SCIP index with its access roles.

No dependencies: decodes the few protobuf fields it needs by hand.
Usage: dump_roles.py index.scip [path-substring]
"""
import sys

ROLES = {1: "definition", 2: "import", 4: "write", 8: "read", 64: "forward-def"}


def varint(buf, i):
    shift = result = 0
    while True:
        b = buf[i]
        i += 1
        result |= (b & 0x7F) << shift
        if not b & 0x80:
            return result, i
        shift += 7


def fields(buf):
    i = 0
    while i < len(buf):
        key, i = varint(buf, i)
        num, wt = key >> 3, key & 7
        if wt == 0:
            val, i = varint(buf, i)
        elif wt == 2:
            n, i = varint(buf, i)
            val, i = buf[i:i + n], i + n
        elif wt == 1:
            val, i = buf[i:i + 8], i + 8
        elif wt == 5:
            val, i = buf[i:i + 4], i + 4
        else:
            raise ValueError(f"wire type {wt}")
        yield num, wt, val


def packed(buf):
    out, i = [], 0
    while i < len(buf):
        v, i = varint(buf, i)
        out.append(v)
    return out


def main():
    data = open(sys.argv[1], "rb").read()
    want = sys.argv[2] if len(sys.argv) > 2 else ""
    for num, _, doc in fields(data):
        if num != 2:  # Index.documents
            continue
        path, occs = "", []
        for dnum, _, dval in fields(doc):
            if dnum == 1:
                path = dval.decode()
            elif dnum == 2:
                occs.append(dval)
        if want not in path:
            continue
        print(f"== {path}")
        rows = []
        for occ in occs:
            rng, sym, roles = [], "", 0
            for onum, owt, oval in fields(occ):
                if onum == 1:
                    rng = packed(oval) if owt == 2 else rng + [oval]
                elif onum == 2:
                    sym = oval.decode()
                elif onum == 3:
                    roles = oval
            names = [n for bit, n in ROLES.items() if roles & bit]
            access = {(True, True): "read+write", (True, False): "read",
                      (False, True): "write"}.get((bool(roles & 8), bool(roles & 4)), "unknown")
            rows.append((rng[0] + 1, rng[1] + 1, access, ",".join(names) or "-", sym))
        for line, col, access, names, sym in sorted(rows):
            print(f"{line:4}:{col:<3} {access:10} {names:22} {sym}")


if __name__ == "__main__":
    main()
