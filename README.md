# SMASH #

Smash is a completely alignment-free tool to find and visualise genomic rearrangements. The detection is based on conditional exclusive compression, namely using a FCM, also known as Markov model, of high context order (typically 20). For visualization, Smash outputs a SVG image, with an ideogram output architecture, where the patterns are represented with several HSV values (only value varies). The following image, illustrating the information maps between human and chimpanzee for the several chromosomes, depicts an example of the compiled runs with Smash: 

![ScreenShot](/HC.png)

## INSTALLATION ##
Cmake is needed for installation (http://www.cmake.org/).
You can download directly from http://www.cmake.org/cmake/resources/software.html or use an appropriate packet manager.

In the following instructions we show the procedure to install, compile and create the information map between human and orangutango chromosome 20:

### STEP 1

Download, install smash and resolve conflits.

#### Linux 
<pre>
sudo apt-get install cmake
wget https://github.com/pratas/smash/archive/master.zip
unzip master.zip
cd smash-master
cmake .
make
</pre>

#### OS X
Only if you don't have brew run:
<pre>
ruby -e "$(curl -fsSL https://raw.github.com/Homebrew/homebrew/go/install)"
</pre>
and after:
<pre>
brew install cmake
brew install wget
brew install gcc48
wget https://github.com/pratas/smash/archive/master.zip
unzip master.zip
cd smash-master
cmake .
make
</pre>

### Step 2 
Download the sequences [the links might change over time].

##### Linux and OS X
<pre>
wget ftp://ftp.ncbi.nlm.nih.gov/genomes/H_sapiens/Assembled_chromosomes/seq/hs_ref_GRCh38_chr20.fa.gz
wget ftp://ftp.ncbi.nlm.nih.gov/genomes/Pongo_abelii/Assembled_chromosomes/seq/pab_ref_P_pygmaeus_2.0.2_chr20.fa.gz
</pre>

### Step 3 
Unzip, exclude headers and filter content.

##### Linux
<pre>
zcat hs_ref_GRCh38_chr20.fa.gz | grep -v ">" | tr -d -c "ACGTN" > HS20
zcat pab_ref_P_pygmaeus_2.0.2_chr20.fa.gz | grep -v ">" | tr -d -c "ACGTN" > PA20
</pre>

##### OS X
<pre>
gzcat hs_ref_GRCh38_chr20.fa.gz | grep -v ">" | tr -d -c "ACGTN" > HS20
gzcat pab_ref_P_pygmaeus_2.0.2_chr20.fa.gz | grep -v ">" | tr -d -c "ACGTN" > PA20
</pre>


### Step 4 ###

Run Smash.

##### Linux, OS X and Windows

<pre>
./smash -v -c 20 -t 1.5 HS20 PA20
</pre>

This step outputs a SVG image using the respective map under the name: HS20PA20.svg (for custom name use option: -o ANYNAME.svg). The respective information map is the following (transformed in a png image and rotated 90 degrees to left):

![ScreenShot](/HS20PA20.png)

## USAGE ##

The Smash program have many options in the interface because there are a wide variety of parameters that can be defined by the user. However, for the detection of the arrangements only two are critical, namely context and threshold. Mathematical information about these parameters can be found in the paper.

To see the possible options type
<pre>
./smash
</pre>
or
<pre>
./smash -h
</pre>
These will print the following options:
<pre>
Usage: smash [OPTIONS]... [FILE] [FILE]      
                                             
 -v                  verbose mode            
 -f                  force (be sure!)        
                                             
 -c  [context]       context order (DEF: 20)
 -t  [threshold]     threshold [0.0,2.0] (DEF: 1.5)
                                             
 -m  [mSize]         minimum block size (DEF: 1000000)
 -i                  show only inverted repeats
 -r  [ratio]         image size ratio (DEF: 1000000)
 -a  [alpha]         alpha estimator (DEF: 1000)
 -s  [seed]          seed for random 'N'     
 -w  [wSize]         window size
 -wt [wType]         window type [0|1|2|3] (DEF: 0)
 -d  [dSize]         sub-sample (DEF: 10000)
 -wi [width]         sequence width (DEF: 25)
                                             
 -o  [outFile]       output svg plot file    
                                             
 [refFile]           reference file          
 [tarFile]           target file      
</pre>

By default, Smash has many parameters assigned in order to avoid the estimation, enabling only to set both reference and target files. However, these defaults are only estimated to detect rearrangements in primates. Therefore, for other purposes you might need to adjust context and threshold parameters. Moreover, for custom image maps you might also need to set other parameters, such as width.

#### Options meaning

| Parameters                     | Meaning                                                                              |
|--------------------------------|:-------------------------------------------------------------------------------------|
|-v           | It will print progress information such as positions of the patterns, times, etc.    |
|-f           | It will force to write over files already created.                                   |
|-c [context] | Size of the FCM (Markov) context order (interval [1;28]). Contexts above 14 will be handled with a hash-table, where the implementation is approximately linear in memory relatively to the size of the sequence. When the sequence is very fragmented, or the species are somehow distant, or the sequencing/assembly process has low quality this value show not be very high. |
| -t [threshold] | It will be used to segment the high from the low regions of information content (interval [0;2]). For distant species this value might be slightly below 2 (such as 1.9). |

## ISSUES ##

For any issue let us know at [issues link](https://github.com/pratas/smash/issues).

## LICENSE ##

GPL v2.

For more information:
<pre>http://www.gnu.org/licenses/gpl-2.0.html</pre>

