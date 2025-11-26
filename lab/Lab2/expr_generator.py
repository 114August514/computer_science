import random
import math

INT_RANGE = (-9, 9)
FLOAT_RANGE = (-9.0, 9.0)

def rand_float(a, b, max_decimals=3):
    x = random.uniform(a, b)
    return round(x, random.randint(0, max_decimals))

def num_literal(float_prob=0.6):
    if random.random() < float_prob:
        v = rand_float(*FLOAT_RANGE)
    else:
        v = random.randint(*INT_RANGE)
    if abs(v) < 1e-12:
        return "0"
    if v < 0:
        s = str(v)
        if '.' in s:
            s = s.rstrip('0').rstrip('.') or "0"
        return f"({s})"
    s = str(v)
    if '.' in s:
        s = s.rstrip('0').rstrip('.') or "0"
    return s

def int_literal(lo, hi):
    k = random.randint(lo, hi)
    return str(k)

def nonzero_literal():
    while True:
        s = num_literal()
        if s not in ("0", "(0)"):
            return s

def gen_expr(n_ops, allow_pow=True):
    if n_ops <= 0:
        return num_literal()

    ops_all = ['+', '-', '*', '/', '^'] if allow_pow else ['+', '-', '*', '/']
    weights = [2, 2, 2, 2, 1] if allow_pow else [2, 2, 2, 2]
    op = random.choices(ops_all, weights=weights, k=1)[0]

    if op == '/':
        left_ops = n_ops - 1
        right_ops = 0
    elif op == '^':
        left_ops = n_ops - 1
        right_ops = 0
    else:
        left_ops = random.randint(0, n_ops - 1)
        right_ops = (n_ops - 1) - left_ops

    left = gen_expr(left_ops, allow_pow=(op != '^'))

    if op == '^':
        exp = int_literal(0, 4)
        right = f"({exp})"
        return f"({left})^{right}"
    elif op == '/':
        right = nonzero_literal()
        return f"({left})/{right}"
    else:
        right = gen_expr(right_ops, allow_pow=True)
        return f"({left}){op}({right})"

def fmt_number(x):
    if math.isclose(x, 0.0, abs_tol=1e-12):
        return "0"
    if isinstance(x, (int,)) or (isinstance(x, float) and x.is_integer()):
        return str(int(x))
    s = f"{x:.12g}"
    if '.' in s:
        s = s.rstrip('0').rstrip('.') or "0"
    return s

def eval_expr(expr_with_caret):
    py = expr_with_caret.replace('^', '**')
    return eval(py, {"__builtins__": {}}, {})

def make_long_valid(base_ops=6):
    return gen_expr(base_ops, allow_pow=True)

def err_paren_mismatch():
    e = make_long_valid(6)
    return "(" + e, "括号不匹配"

def err_div_by_zero():
    L = make_long_valid(4)
    R = make_long_valid(3)
    e = f"({L})/0+({R})"
    return e, "除以0"

def err_zero_negative_power():
    k = random.randint(1, 3)
    tail = make_long_valid(3)
    e = f"0^(-{k})+({tail})"
    return e, "0的负数次方"

def err_consecutive_ops():
    e = make_long_valid(6)
    idx = -1
    for i, ch in enumerate(e):
        if ch in "+*/^":
            idx = i
            break
    if idx == -1:
        e2 = "+" + e
    else:
        op2 = random.choice("+-*/^")
        e2 = e[:idx+1] + op2 + e[idx+1:]
    return e2, "运算符连续出现"

def make_valid_random():
    e = gen_expr(random.randint(6, 10), allow_pow=True)
    if "^(" in e and random.random() < 0.25:
        parts = e.split("^(")
        head, tail = "^(".join(parts[:-1]), parts[-1]
        k_str = ""
        for ch in tail:
            if ch.isdigit() or ch == '-':
                k_str += ch
            else:
                break
        base_is_zero = head.endswith("(0)")
        if k_str and not base_is_zero:
            k = int(k_str)
            if k == 0:
                k = 1
            tail = tail.replace(k_str, f"-{abs(k)}", 1)
            e = head + "^(" + tail
    val = eval_expr(e)
    return e, val

def main():
    # random.seed(42)
    err1, why1 = err_paren_mismatch()
    err2, why2 = err_div_by_zero()
    err3, why3 = err_zero_negative_power()
    err4, why4 = err_consecutive_ops()

    print(f"{err1}=<错误:{why1}>")
    print(f"{err2}=<错误:{why2}>")
    print(f"{err3}=<错误:{why3}>")
    print(f"{err4}=<错误:{why4}>")

    for _ in range(6):
        try:
            expr, val = make_valid_random()
            print(f"{expr}={fmt_number(val)}")
        except Exception as e:
            expr, val = make_valid_random()
            print(f"{expr}={fmt_number(val)}")

if __name__ == "__main__":
    main()
