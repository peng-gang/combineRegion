# combineRegion

This program is used to combine the overlapped or adjacent regions in a bed file together. It can also be used to sort the regions in a bed file accoring to chromosome, start and end position. 

## usage
Download the source files and complie them with g++ like  
g++ *.cpp -o combineRegion

After compiling, you can type the following command to run the program:  
combineRegion input.bed output.bed

If you want to sort the bed file without combining the ovelapped and adjacent regions, type:  
combineRegion input.bed output.bed NC

## Example
###Combine the overlapped and adjacent regions

Input bed file:  
chr1	200	500  
chr1	1000	1400  
chr2	100	300  
chr1	1300	1650  
chr1	1600	2010  
chr2	301	500  

Output bed file:  
chr1	200	500  
chr1	1000	2010  
chr2	100	500  

###Sort only
Input bed file:  
chr1	200	500  
chr1	1000	1400  
chr2	100	300  
chr1	1300	1650  
chr1	1600	2010  
chr2	301	500  

Output bed file:  
chr1	200	500  
chr1	1000	1400  
chr1	1300	1650  
chr1	1600	2010  
chr2	100	300  
chr2	301	500  
