
n_X <- 10
n_Y <- 15
mu_X <- 5
sigma_X <- sqrt(8)
mu_Y <- -2
sigma_Y <- sqrt(16)

num_simulations <- 10000

V_samples <- numeric(num_simulations)

for (i in 1:num_simulations) {
  X_sample <- rnorm(n_X, mean = mu_X, sd = sigma_X)
  Y_sample <- rnorm(n_Y, mean = mu_Y, sd = sigma_Y)
  
  V_samples[i] <- mean(X_sample) - mean(Y_sample)
}

mean_V <- mean(V_samples)
var_V <- var(V_samples)

proportion_between_6_and_9 <- mean(V_samples >= 6 & V_samples <= 9)

cat("Mean of V:", mean_V, "\n")
cat("Variance of V:", var_V, "\n")
cat("Proportion of V between 6 and 9:", proportion_between_6_and_9, "\n")

hist(V_samples, probability = TRUE, breaks = 30, 
     main = "Histogram of Sampled Values of V with PDF",
     xlab = "V")