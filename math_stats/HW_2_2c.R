# z1 <- (3.95-4) / sqrt(16/30)
# z2 <- (4.05-4) / sqrt(16/30)

# z1 <- (3.95-(40/11)) / sqrt(160/1452)
# z2 <- (4.05-(40/11)) / sqrt(160/1452)
# result <- pnorm(z2) - pnorm(z1)
# cat("result <- pnorm(z2) - pnorm(z1) =", result, "\n")


n <- 10      
b <- 4         
n_sim <- 10000 

b1_values <- numeric(n_sim)
b2_values <- numeric(n_sim)
b3_values <- numeric(n_sim)

for (i in 1:n_sim) {

  sample <- runif(n, min = 0, max = b)
  
  b1_values[i] <- 2 * mean(sample)              
  b2_values[i] <- max(sample)                   
  b3_values[i] <- (n + 1) / n * max(sample)     
}

prop_b1 <- mean(abs(b1_values - b) <= 0.05)
prop_b2 <- mean(abs(b2_values - b) <= 0.05)
prop_b3 <- mean(abs(b3_values - b) <= 0.05)

cat("Proportion of values within 0.05 of b:\n")
cat("Estimator b1 (2 * X̄):", prop_b1, "\n")
cat("Estimator b2 (X(n)):", prop_b2, "\n")
cat("Estimator b3 ((n+1)/n * X(n)):", prop_b3, "\n")
