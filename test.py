import mypackage.bayesopt_tpe as bayesopt_tpe

def objective_function(params):
    x, y = params
    return (x - 1) ** 2 + (y - 2) ** 2

space_min = [0.0, 0.0]
space_max = [5.0, 5.0]
best_params = bayesopt_tpe.bayesopt_tpe(
    objective_function,
    space_min,
    space_max,
    iterations=50.0,
    samples=20.0,
    acquisition_samples=50.0,
    verbose=1,
    bandwidth=[0.1, 0.1]
)

print("Best parameters:", best_params)
