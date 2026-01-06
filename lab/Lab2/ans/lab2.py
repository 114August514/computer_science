Q = [0] * 101

for i in range(101):
    if (i <= 2):
        Q[i] = 1
        continue

    t1 = i - Q[i-1]
    t2 = i - Q[i-2]
    if (t1 < 0 or t2 < 0):
        print("111")

    Q[i] = Q[t1] + Q[t2]
    
def H_Q(num: int) -> int:
    if (num <= 0 or num > 100):
        return -1

    if (num == 1 or num == 2):
        return 1
    
    q = [0] * num
    q[0] = q[1] = 1
    temp1 = temp2 = 1
    i = 3
    while i <= num:
        p1 = i - temp1
        p2 = i - temp2
        q[i-1] = q[p1-1] + q[p2-1]
        temp1 = temp2
        temp2 = q[i-1]
        i += 1

    return q[num-1]

for i in range(1, 101):
    if Q[i] == H_Q(i):
        print(f"Q[{i}]: {Q[i]}", "bc")
        continue
    else:
        print(i)

print("111")