n_simulations <- 10000
n_rolls <- 100

chi_squared_values <- numeric(n_simulations)

for (i in 1:n_simulations) {
  sample_rolls <- sample(1:6, n_rolls, replace = TRUE)
  
  observed_counts <- table(factor(sample_rolls, levels = 1:6))
  
  expected_count <- n_rolls / 6
  
  chi_squared_values[i] <- sum((observed_counts - expected_count)^2 / expected_count)
}

hist(chi_squared_values, main = "Histogram of Chi-Squared Values (Simulations)",
     xlab = "Chi-Squared Value", col = "lightblue", breaks = 50)

observed_chi_squared <- 11.24
p_value <- mean(chi_squared_values >= observed_chi_squared)

cat("Proportion of samples with chi-squared value at least as large as the observed:", p_value, "\n")

