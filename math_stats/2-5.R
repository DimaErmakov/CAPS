#For documentation
?pnorm

#To execute a command/routine
#ctrl+enter line by line, or first select lines to run

#####
#A few basic things
##Arithmetic
3+7
3-7
3*7
3/7
3^7

##Creating a vector
v <- c(1,7,8.5,11,20,37)
v
w <- 1:6
w
u <- seq(from = 0, to = 10, by = 0.5)
u

v+w               #entrywise sum
v*w               #entrywise product of elements
v%*%w             #dot product

##Accessing elements of a vector
v[2]
v[3:6]

v[v >= 20]          #select elements of v greater than or equal to 20
v[v != 20]         #select elements of v not equal to 20

##Arithmetic is "vectorized"
v
v+2               #add 2 to each element of v
v^2               #square each element of v
sqrt(v)           #take square root of each element of v

##Some vector calculations
v
sum(v)            #sum elements of v
sum(v^2)          #sum squares of elements of v
sum(v[v > 20])    #sum elements of v greater than 20

mean(v)           #mean of elements of v
mean(v >= 20)     #proportion of elements of v greater than 20
mean(v[v >= 20])  #mean of the elements of v that are greater than or equal to 20
mean(v != 20)     #proportion of elements not equal to 20
mean((v >= 8) & (v <= 20))    #proportion of elements of v at least 8 and at most 20
#####

#####
#Built-in probability distributions
##Poisson
lambda <- 1
pexp(1,rate=lambda)         #P(X <= 1) (CDF)
pexp(.8,rate=lambda) - pexp(.6,rate=lambda)  #P(.6 <= X <= .8)
dexp(1,rate=lambda)       #value of PDF at 1
qexp(.5,rate=lambda)      #.5 quantile (median)
rexp(5,rate=lambda)       #random sample of size 5

##Normal
pnorm(15,mean=17,sd=4)

1-pnorm(15,mean=17,sd=4)

##Binomial
dbinom(22,size=40,prob=.5)
pbinom(10,size=40,prob=.5)

#Random sampling from distribution, using normal as example
samp <- rnorm(1000,mean=1,sd=2)
hist(samp,freq=FALSE,main='Random Sample of Size 1000 from Exp(1)',xlab='Sample Values',col='blue',breaks=20)  #make histogram
curve(dnorm(x,mean=1,sd=2),col = 'red',lwd = 2,add=TRUE)   #overlay PDF of normal

mean(samp <= 5)    #proportion of sample values at most 5
pnorm(5,mean=1,sd=2)     #exact probability

#Distribution of the sample mean for 1000 random samples of size 10 from B(1,p) distribution
N <- 10^3
p <- .6
prop <- numeric(N)   #creates vector of length N for storing sampled values
for(i in 1:N){
  samp <- rbinom(1,size = 50,prob = .8)
  prop[i] <- mean(samp)
}

##Plot histogram of proportion estimates for the 1000 samples
hist(prop,main = paste('Histogram of Sample Means\n',N,'Samples of Size 50'),xlab = 'Sample Mean',ylab = 'Count',col = 'blue',breaks=20)
abline(v = p,col = 'red',lwd=2)    #Plot vertical line at true value of p

mean(prop)   #mean of sampled proportions
var(prop)    #variance of sampled proportions
sd(prop)     #standard deviation of sampled proportions

v <- 1:50
s <- sqrt(v)
sum(s)
1-pnorm(17,mean=17,sd=4)
samp <- rexp(1000,0.25)
hist(samp,freq=FALSE,main='Random Sample of Size 1000 from Exp(1)',xlab='Sample Values',col='blue',breaks=20)  #make histogram
curve(dnorm(x,mean=1,sd=2),col = 'red',lwd = 2,add=TRUE)   #overlay PDF of normal

samp <- rpois(1000,4)
hist(samp,freq=FALSE,main='Random Sample of Size 1000 from Pois(4)',xlab='Sample Values',col='blue',breaks=20)  #make histogram
curve(ppois(x,4),col = 'red',lwd = 2,add=TRUE)   #overlay PDF of normal

samp <- rbinom(1000,4)

