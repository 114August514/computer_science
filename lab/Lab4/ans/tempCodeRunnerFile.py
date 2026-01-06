def rececursive_for_routes(N: int, M: int):
    if (N == 0 or M == 0):
        return 1
    return rececursive_for_routes(N-1, M) + rececursive_for_routes(N, M-1)

def calculate_recommendations(N: int=0, M: int=0):
    steps = N + M
    routes = rececursive_for_routes(N, M)
    return 5 * routes - steps