import sys
from subprocess import run

maps=r'''
1 2 3 4 5  6 7 8 9 0  --  ! @ # $ %  ^ & * ( )  --  ! @ # $ %  ^ & * ( )
' , . p y  f g c r l  --  " < > P Y  F G C R L  --  < = > | %  f g : + l
a o e u i  d h t n s  --  A O E U I  D H T N S  --  ( [ { ' `  d " } ] )
  q j k x  b m w v    --    Q J K X  B M W V    --    q j \ ~  b * w v
j -- J -- Return
Escape -- Escape -- space
'''

# ({['":  '"]}) .,
# => <= += !=
# ({['":'"]})
# x = {'aoeu': "aoeu"}
# x = ['aoeu', "aoeu"]

maps = maps.strip().split('\n')

def U(x):
    if len(x) == 1 and not x.isalnum():
        return f'U{ord(x):X}'
    else:
        return x.rjust(3)

cmds = []

for row in maps:
    if not row.strip():
        continue
    parts = row.strip().split('--')
    for keys in zip(*[ part.split() for part in parts ]):
        keys = list(keys)
        if len(keys) == 3:
            keys += [keys[-1]]
        print(*keys)
        cmd = 'keysym', U(keys[0]), '=', *map(U, keys)
        cmd = ' '.join(cmd)
        cmds += [cmd]

cmds = '\n'.join(cmds)
print(cmds)
run(['xmodmap', '-'], input=cmds, text=True)
