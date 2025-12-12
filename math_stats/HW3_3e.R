p <- 0.3
n <- 30
generate_phat <- function(n, p) { # generate one sample of p-hat
  x <- rgeom(n, p) + 1  
  xbar <- mean(x)    
  phat <- 1 / xbar    
  return(phat)
}
phat_samples <- replicate(10000, generate_phat(n, p))

sim_mean <- mean(phat_samples)
sim_bias <- sim_mean - p
sim_variance <- var(phat_samples)
theoretical_bias <- p * (1 - p) / n
theoretical_variance <- p^2 * (1 - p) / n

cat("Simulated Mean of p-hat:", sim_mean, "\n")
cat("Simulated Bias of p-hat:", sim_bias, "\n")
cat("Theoretical Bias:", theoretical_bias, "\n")
cat("Simulated Variance of p-hat:", sim_variance, "\n")
cat("Theoretical Variance:", theoretical_variance, "\n")
cat("Bias Difference:", sim_bias - theoretical_bias, "\n")
cat("Variance Difference:", sim_variance - theoretical_variance, "\n")
