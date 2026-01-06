def rececursive_for_routes(N: int, M: int):
    if (N == 0 or M == 0):
        return 1
    return rececursive_for_routes(N-1, M) + rececursive_for_routes(N, M-1)

def calculate_recommendations(N: int=0, M: int=0):
    steps = N + M
    routes = rececursive_for_routes(N, M)
    return 5 * routes - steps

def calculate_recommendations2(N: int=0, M: int=0):
    steps = N + M

    dp = [[0] * (M + 1) for _ in range(N + 1)]
    for i in range(N + 1):
        for j in range(M + 1):
            if (i * j == 0):
                dp[i][j] = 1
            else:
                dp[i][j] = dp[i - 1][j] + dp[i][j - 1]
    return dp[N][M] * 5 - steps

for i in range(0, 6):
    for j in range(0, 6):
        cal1 = calculate_recommendations(i, j)
        cal2 = calculate_recommendations2(i, j)
        print(f"当 N = {i}, M = {j} 时, 推荐值为{cal2}")
        if (cal1 != cal2):
            print("================================")