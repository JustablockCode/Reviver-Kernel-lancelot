#!/usr/bin/env python3
"""Fail if Kconfig silently drops a requested NetHunter option or root feature."""
import pathlib
import sys

root = pathlib.Path(__file__).resolve().parents[2]
resolved = set(pathlib.Path(sys.argv[1]).read_text().splitlines())
fragment = root / 'arch/arm64/configs/vendor/lancelot-nethunter.config'
required = [line for line in fragment.read_text().splitlines()
            if line.startswith('CONFIG_')]
required += [f'CONFIG_{key}=y' for key in (
    'KSU', 'KSU_SUSFS', 'KSU_SUSFS_PRIORITY_UID_FILTER', 'KPM',
    'KSU_FEATURE_ADBROOT', 'KALLSYMS', 'KALLSYMS_ALL', 'NOMOUNT')]
missing = [line for line in required if line not in resolved]
if missing:
    sys.exit('Unresolved required configuration:\n' + '\n'.join(missing))
print(f'NetHunter/root configuration: {len(required)} requirements passed')
