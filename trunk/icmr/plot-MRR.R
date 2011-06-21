require(psych)
require(Hmisc)
#
# individual datasets
#
Datasets <- array( c("Random", "CO-1", "StripeCodes",
                     "result-random.txt",
					 "result-MRHistograms.txt",
					 "result-StripeCodes.txt"
					 ), dim=c(3,2))

#
# Read datasets
#
nDatasets <- dim(Datasets)[1]
data <- dim(nDatasets)
for(i in c( 1:nDatasets ) )  {
	data[[i]] <- read.table(Datasets[i,2], header=T)
	print(paste(Datasets[i,1],Datasets[i,2]))
}

#
# Get x-axis values and do a sanity check across datasets
# that they match
#
Xmarkers <- unique(data[[1]]$dbsize)
for(i in c( 2:nDatasets ) )
	if(length( unique(unique(data[[i]]$dbsize) == Xmarkers)) != 1)
		print("ERROR! Mismatch in x-values in dataset.")

#
# Aggregate the mean reciprocal rank and its variance across datasets
#
stderr <- function(x) sqrt(var(x)/length(x))
ymin <- 1000000
ymax <- -100
mRR <- dim(nDatasets)
sdRR <- dim(nDatasets)
for(i in c( 1:nDatasets ) ) {
	agg1 <- aggregate(data[[i]], by=list(data[[i]]$dbsize), FUN=mean)
	agg2 <- aggregate(data[[i]], by=list(data[[i]]$dbsize), FUN=stderr)

	mRR[[i]]  <- agg1["reciprank"][,1]
	sdRR[[i]] <- agg2["reciprank"][,1] 

	y1 <- min(mRR[[i]]) - max(sdRR[[i]])
	y2 <- max(mRR[[i]]) + max(sdRR[[i]])
	if(y1 < ymin) 
		ymin <- y1
	if(y2 > ymax)
		ymax <- y2
}

#
# Set up output
#
postscript("fig-MRR.eps", horizontal=FALSE, width=4, height=3, paper="special", onefile=FALSE)
par(oma=c(0,0,0,0))
par(mgp=c(1.9,0.5,0))
par(mar=c(3,2.9,0.7,0.6))
par(mai=c(.6,.6,.2,.2))
#png("fig-MRR.png")

#
# Plot 
#
legendStr <- c()
for(i in c( 1:nDatasets ) ) {
	if(i == 1) 
		plot(mRR[[1]] ~ Xmarkers, xlab="Number of animals", ylab="Mean reciprocal rank (MRR)", axes=T, 
    		 ylim=c(ymin,1), xaxt="n")
	else
		points(mRR[[i]] ~ Xmarkers)
	
	lines(mRR[[i]] ~ Xmarkers, lty = i)
	errbar(Xmarkers, mRR[[i]], mRR[[i]]+sdRR[[i]], mRR[[i]]-sdRR[[i]], add=T)	

	legendStr <- c(legendStr, Datasets[i,1])
}
axis(1, at=Xmarkers)
legend(x="topright", bty="n", legend=legendStr, lty=c( 1:nDatasets ) );
rc = dev.off()


