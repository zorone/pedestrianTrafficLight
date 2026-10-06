count = { 'strict_pass': 0, 'strict_fail': 0, 'loosen_pass': 0, 'loosen_fail': 0}
for i in range(256):
    for j in range(256):
        k = i - j
        res = int()
        if(k >= 0):
            res = k
        else:
            res = 256 + k
        t = ((i - j) < (~k))

        if(k >= 0):
            if(t):
                print(f'{i:3} - {j:3} = {k:4} ({res:4}): {res:3} < {j:4}: STRICTLY PASS')
                count['strict_pass'] += 1
            else:
                print(f'{i:3} - {j:3} = {k:4} ({res:4}): {res:3} < {j:4}: STRICTLY FAIL')
                count['strict_fail'] += 1
        else:
            if(abs(i - j) >= 128):
                if(t):
                    print(f'{i:3} - {j:3} = {k:4} ({res:4}): {res:3} < {j:4}: LOOSENLY PASS')
                    count['loosen_pass'] += 1
                else:
                    print(f'{i:3} - {j:3} = {k:4} ({res:4}): {res:3} < {j:4}: LOOSENLY FAIL (delta: {abs(k)-res:4}')
                    count['loosen_fail'] += 1
            else:
                if(t):
                    print(f'{i:3} - {j:3} = {k:4} ({res:4}): {res:3} < {j:4}: STRICTLY PASS')
                    count['strict_pass'] += 1
                else:
                    print(f'{i:3} - {j:3} = {k:4} ({res:4}): {res:3} < {j:4}: STRICTLY FAIL (delta: {abs(k)-j:4}')
                    count['strict_fail'] += 1
print(f'STRICTLY PASS: {count['strict_pass']}')
print(f'STRICTLY FAIL: {count['strict_fail']}')
print(f'LOOSENLY PASS: {count['loosen_pass']}')
print(f'LOOSENLY FAIL: {count['loosen_fail']}')
