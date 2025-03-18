library(ggplot2)

df <- 10  
mean_W <- df  
var_W <- 2 * df 

p_6_to_9 <- pchisq(9, df) - pchisq(6, df)
cat("P(6 < W < 9):", p_6_to_9, "\n\n")

n_sim <- 10000 
n_samples <- 10  

W_samples <- replicate(n_sim, {
  X <- rnorm(n_samples, mean = 5, sd = sqrt(8))
  Z <- (X - 5) / sqrt(8)
  sum(Z^2)
})

mean_sim_W <- mean(W_samples)
var_sim_W <- var(W_samples)
prop_6_to_9 <- mean(W_samples > 6 & W_samples < 9)

cat("Simulation results:\n")
cat("Mean of sampled W:", mean_sim_W, "\n")
cat("Variance of sampled W:", var_sim_W, "\n")
cat("Proportion of W values between 6 and 9:", prop_6_to_9, "\n\n")

hist_data <- data.frame(W = W_samples)

ggplot(hist_data, aes(x = W)) +
  geom_histogram(aes(y = after_stat(density)), bins = 50, color = "black", fill = "lightblue", alpha = 0.7) +
  stat_function(fun = dchisq, args = list(df = df), color = "red", linewidth = 1) +
  labs(title = "Histogram of Simulated W with Chi-squared PDF",
       x = "W",
       y = "Density")
