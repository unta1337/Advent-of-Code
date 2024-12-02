import sympy as sp

# Define the variables for t1, t2, t3, t4, t5 as symbols
p, q, r, i, j, k = sp.symbols('p q r i j k')
t1, t2, t3, t4, t5 = sp.symbols('t1 t2 t3 t4 t5')

# Re-define the system of equations without treating t1-t5 as constants
eq1 = sp.Eq(i * t1 + p, -2 * t1 + 19)
eq2 = sp.Eq(j * t1 + q,  1 * t1 + 13)
eq3 = sp.Eq(k * t1 + r, -2 * t1 + 30)

eq4 = sp.Eq(i * t2 + p, -1 * t2 + 18)
eq5 = sp.Eq(j * t2 + q, -1 * t2 + 19)
eq6 = sp.Eq(k * t2 + r, -2 * t2 + 22)

eq7 = sp.Eq(i * t3 + p, -2 * t3 + 20)
eq8 = sp.Eq(j * t3 + q, -2 * t3 + 25)
eq9 = sp.Eq(k * t3 + r, -4 * t3 + 34)

eq10 = sp.Eq(i * t4 + p, -1 * t4 + 12)
eq11 = sp.Eq(j * t4 + q, -2 * t4 + 31)
eq12 = sp.Eq(k * t4 + r, -1 * t4 + 28)

eq13 = sp.Eq(i * t5 + p, 1 * t5 + 20)
eq14 = sp.Eq(j * t5 + q, -5 * t5 + 19)
eq15 = sp.Eq(k * t5 + r, -3 * t5 + 15)

# Solve the system for i, j, k, p, q, r
solution = sp.solve([eq1, eq2, eq3, eq4, eq5, eq6, eq7, eq8, eq9, eq10, eq11, eq12, eq13, eq14, eq15], [i, j, k, p, q, r, t1, t2, t3, t4, t5], dict=True)

print(solution)
