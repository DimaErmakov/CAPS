
from sklearn.preprocessing import StandardScaler
from sklearn.linear_model import Ridge
from sklearn.pipeline import Pipeline
import pandas as pd
import io
from scipy.optimize import curve_fit
from sklearn.model_selection import train_test_split
import numpy as np
import matplotlib.pyplot as plt
import pandas as pd

df1 = pd.read_csv(r"C:\Users\ermak\Downloads\polys.csv").dropna()


g = lambda x : 3*x + 2

n:int = 2**7
n_reps:int = 2**10

magnitude_of_noise:float = 0.5

x_plt = np.linspace(0., 1., 2**10)

ALPHA = 0.025

def rmse(x,y) -> float:
  return np.mean( (x - y)**2 )**0.5

df = pd.DataFrame({'rmses_fancy_train':[], 'rmses_fancy_val':[], 'rmses_baby_bear_train':[], 'rmses_baby_bear_val':[]})


for i in range(n_reps):
  x = np.random.uniform(0., 1., n)
  eps = magnitude_of_noise*np.random.normal(0., 1., n)
  y = g(x) + eps
  # 20% hold-out data. see note below on the % held out for validation:
  x_train,x_val,y_train,y_val = train_test_split(x,y, test_size=0.2)
  
  f_fancy = lambda x,a6,a5,a4,a3,a2,a1,a0 : a6*x**6 + a5*x**5 + a4*x**4 + a3*x**3 + a2*x**2 + a1*x + a0
  params_fancy,_ = curve_fit(f_fancy, x_train, y_train)
  plt.plot(x_plt, f_fancy(x_plt, *params_fancy), color='red', alpha=ALPHA, label='higher polynomial' if i==0 else '_')

  f_baby_bear = lambda x,a1,a0 : a1*x + a0
  params_baby_bear,_ = curve_fit(f_baby_bear, x_train, y_train)
  plt.plot(x_plt, f_baby_bear(x_plt, *params_baby_bear), color='blue', alpha=ALPHA, label='linear' if i==0 else '_')

  df.loc[len(df)] = {'rmses_fancy_train':rmse(f_fancy(x_train, *params_fancy), y_train), 'rmses_fancy_val':rmse(f_fancy(x_val, *params_fancy), y_val), 'rmses_baby_bear_train':rmse(f_baby_bear(x_train, *params_baby_bear), y_train), 'rmses_baby_bear_val':rmse(f_baby_bear(x_val, *params_baby_bear), y_val)}

plt.plot(x_plt, g(x_plt), label=r'Truth, $g$', color='black')

legend = plt.legend()

# make legend alpha = 1
for line in legend.get_lines():
  line.set_alpha(1.0)

print(df.describe())

# NOTE: if we use a larger validation proportion, we'll get a higher
# chance of choosing the correct model family. but we'd also have less
# data to train on, so our model estimate from the correct model
# family would be of lower quality. think of a high validation % as
# being good for choosing the correct model family and a low
# validation % as being good for choosing the correct model among that
# family; however, after choosing the a good model family, if we train
# that model family again with a lower validation %, the resulting
# model might change, and we won't have as much confidence in our
# validation to confirm whether that change is good or not. it's a
# tradeoff.
print(f'How often did the training error tell us to choose the linear "baby bear" model? {100*(df.rmses_baby_bear_train < df.rmses_fancy_train).mean():.2f}%')
print(f'How often did the validation error tell us to choose the linear "baby bear" model? {100*(df.rmses_baby_bear_val < df.rmses_fancy_val).mean():.2f}%')

plt.show()
