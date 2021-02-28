import sys
from subprocess import run

Quiet  = '-q' in sys.argv
DryRun = '-n' in sys.argv

maps = {}
maps[10] = r'''  1 2 3 4 5  6 7 8 9 0  --  ! @ # $ %  ^ & * ( )  --  ! , . $ %  ^ & * ( )  '''
maps[24] = r'''  ' , . p y  f g c r l  --  " < > P Y  F G C R L  --  < = > | %  f g : + l  '''
maps[38] = r'''  a o e u i  d h t n s  --  A O E U I  D H T N S  --  ( [ { ' `  d " } ] )  '''
maps[52] = r'''  : q j k x  b m w v z  --  ; Q J K X  B M W V Z  --  : q j \ ~  b * w v z  '''
maps[54] = r'''  j -- J -- Return  '''
maps[9]  = r'''  Escape -- Escape -- space  '''

# ({['":  '"]}) .,
# => <= += !=
# ({['":'"]})
# x = {'aoeu': "aoeu"}
# x = ['aoeu', "aoeu"]
# x = ['aoeu', "aoeu"]
# y = {'aoeu': "aoeu"}
# ({['": '"]})

def U(x):
    if len(x) == 1 and not x.isalnum():
        return f'U{ord(x):X}'
    else:
        return x.rjust(3)

cmds = []

for start, row in maps.items():
    parts = row.strip().split('--')
    for code, keys in enumerate(zip(*[ part.split() for part in parts ]), start=start):
        keys = list(keys)
        if len(keys) == 3:
            keys += [keys[-1]]
        if not Quiet:
            print(*keys)
        cmd = 'keycode', str(code), '=', *map(U, keys)
        cmd = ' '.join(cmd)
        cmds += [cmd]

cmds = '\n'.join(cmds)
if not Quiet:
    print(cmds)
if not DryRun:
    run(['xmodmap', '-'], input=cmds, text=True)
