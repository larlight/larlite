import subprocess,sys

try:
    flag=[x for x in subprocess.getoutput('root-config --cflags').split() if x.startswith('-std=c++')]
    if len(flag) == 1:
        sys.stdout.write(flag[0])
    else:
        sys.stdout.write('')
    sys.stdout.flush()
    sys.exit(0)
except Exception:
    sys.stdout.write('')
    sys.stdout.flush()
    sys.exit(1)
