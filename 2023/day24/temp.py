from sympy import *

# Let's define the parametric line equation: L(t) = (x0, y0, z0) + t * (a, b, c)
# We will create a system of equations for each projectile line and then solve for the unknowns

# Variables for the unknowns: initial position (x0, y0, z0) and velocity (a, b, c)
x0, y0, z0, a, b, c = symbols('x0 y0 z0 a b c')
t1, t2, t3, t4, t5 = symbols('t1 t2 t3 t4 t5')

# Define the parametric equations for the projectiles s1, s2, s3, s4, s5
# s1(t) = (19, 13, 30) + (-2, 1, -2) * t
# s2(t) = (18, 19, 22) + (-1, -1, -2) * t
# s3(t) = (20, 25, 34) + (-2, -2, -4) * t
# s4(t) = (12, 31, 28) + (-1, -2, -1) * t
# s5(t) = (20, 19, 15) + (1, -5, -3) * t

# Parametric line equation L(t) = (x0, y0, z0) + t * (a, b, c)
L_t1 = Matrix([x0 + a * t1, y0 + b * t1, z0 + c * t1])
L_t2 = Matrix([x0 + a * t2, y0 + b * t2, z0 + c * t2])
L_t3 = Matrix([x0 + a * t3, y0 + b * t3, z0 + c * t3])
L_t4 = Matrix([x0 + a * t4, y0 + b * t4, z0 + c * t4])
L_t5 = Matrix([x0 + a * t5, y0 + b * t5, z0 + c * t5])

# Equations for each projectile line to match the parametric line L(t)
eq_s1 = Eq(Matrix([19, 13, 30]) + t1 * Matrix([-2, 1, -2]), L_t1)
eq_s2 = Eq(Matrix([18, 19, 22]) + t2 * Matrix([-1, -1, -2]), L_t2)
eq_s3 = Eq(Matrix([20, 25, 34]) + t3 * Matrix([-2, -2, -4]), L_t3)
eq_s4 = Eq(Matrix([12, 31, 28]) + t4 * Matrix([-1, -2, -1]), L_t4)
eq_s5 = Eq(Matrix([20, 19, 15]) + t5 * Matrix([1, -5, -3]), L_t5)

# Solve the system of equations for x0, y0, z0, a, b, c, t1, t2, t3, t4, t5
system_of_equations = [eq_s1, eq_s2, eq_s3, eq_s4, eq_s5]

# Solve for the unknowns
solution = solve(system_of_equations, [x0, y0, z0, a, b, c, t1, t2, t3, t4, t5], dict=True)
print(solution)
