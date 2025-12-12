n <- 10    
p <- 0.1  

simulate_confidence_interval <- function(n, p) {
  sample <- rbinom(n, 1, p)
  
  p_hat <- (2 + sum(sample)) / (4 + n)
  
  standard_error <- sqrt(p_hat * (1 - p_hat) / (n + 4))
  
  lower_bound <- p_hat - 1.96 * standard_error
  upper_bound <- p_hat + 1.96 * standard_error
  
  contains_p <- (lower_bound <= p) & (upper_bound >= p)
  
  return(contains_p)
}

results <- replicate(10000, simulate_confidence_interval(n, p))
probability_containing_p <- mean(results)

cat("Probability that the adjusted interval contains p: ", probability_containing_p, "\n")
