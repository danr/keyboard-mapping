import sys
from subprocess import run
import kakmap

Quiet  = '-q' in sys.argv
DryRun = '-n' in sys.argv

maps: dict[int, str] = {}
maps[10] = r'''  1 2 3 4 5  6 7 8 9 0  --  ! @ # $ %  ^ & * ( )  --  ! , . $ %  ^ & * ( )  '''
maps[24] = r'''  ' , . p y  f g c r l  --  " < > P Y  F G C R L  --  < = > | %  f g : + l  '''
maps[38] = r'''  a o e u i  d h t n s  --  A O E U I  D H T N S  --  ( [ { ' `  d " } ] )  '''
maps[52] = r'''  : q j k x  b m w v z  --  ; Q J K X  B M W V Z  --  å ä ö \ ~  b * w v z  '''
maps[9]  = r'''  Escape -- Escape -- space  '''

# ({['":  '"]}) .,
# => <= += !=
# ({['":'"]})
# x = {'aoeu': "aoeu"}
# x = ['aoeu', "aoeu"]
# x = ['aoeu', "aoeu"]
# y = {'aoeu': "aoeu"}
# ({['": '"]})

def U(x: str):
    if len(x) == 1 and (not x.isalnum() or ord(x) > 128):
        return f'U{ord(x):X}'
    else:
        return x.rjust(3)

cmds: list[str] = []

for start, row in maps.items():
    parts = row.strip().split('--')
    for code, keys in enumerate(zip(*[ part.split() for part in parts ]), start=start):
        keys = list(keys)
        if len(keys) == 3:
            keys += [keys[-1]]
        head, *_ = keys
        if head in kakmap.m:
            keys[2] = kakmap.m[head]
            keys[3] = kakmap.m[head]
        *_, last = keys
        if len(last) == 1:
            keys[3] = last.upper()
        if not Quiet:
            print(*keys)
        cmd = 'keycode', str(code), '=', *map(U, keys)
        cmd = ' '.join(cmd)
        cmds += [cmd]

input = '\n'.join(cmds)
if not Quiet:
    print(input)
if not DryRun:
    run(['xmodmap', '-'], input=input, text=True)
