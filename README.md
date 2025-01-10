# DSA-Pagerank-Project

Background

In this assignment, your task is to implement a simple search engine using a simplified version of the well-known Weighted PageRank algorithm. You should start by reading the Wikipedia article on PageRank (read up to the section Damping Factor). Later I will release a video lecture introducing this assignment and discuss the required topics.

The main focus of this assignment is to build a graph structure from a set of web pages, calculate Weighted PageRanks and rank pages. To make things easier for you, you don't need to spend time crawling, collecting and parsing web pages for this assignment. Instead, you will be provided with a collection of mock web pages in the form of plain text files. Each web page has two sections:

Section 1 contains URLs representing outgoing links. The URLs are separated by whitespace, and may be spread across multiple lines.
Section 2 contains the actual content of the web page, and consists of one or more words. Words are separated by whitespace, and may be spread across multiple lines.
Here is an example web page:

   
#start Section-1

url2  url34  url1 url26
  url52 url21
url74  url6 url82

#end Section-1

#start Section-2

Mars has long been the subject of human interest. Early telescopic observations
revealed color changes on the surface that were attributed to seasonal vegetation
and apparent linear features were ascribed to intelligent design.

#end Section-2

Sections will always be appropriately delimited by lines containing #start Section-1, #end Section-1, #start Section-2 and #end Section-2. There will be no other characters on these lines (no leading or trailing whitespace). There may be lines containing whitespace before or after any section. Web pages contain exactly one of each section, and Section 1 will always precede Section 2.

Note that it is completely valid for Section 1 to be empty - this simply means that the web page does not have any outgoing links.

Part 1 - Weighted PageRanks

Your task is to implement a program in a file called pagerank.c that calculates Weighted PageRanks for a collection of pages.

The URLs of the pages in the collection are given in the file collection.txt. The URLs in collection.txt are separated by whitespace, and may be spread across multiple lines. Here is an example collection.txt:

url25   url31 url2
   url102   url78
url32  url98 url33
To get the name of the file that contains the page associated with some URL, simply append .txt to the URL. For example, the page associated with the URL url24 is contained in url24.txt.

For each URL in the collection file, you need to read its corresponding page and build a graph structure using a graph representation of your choice. Then, you must use the algorithm below to calculate the Weighted PageRank for each page.

The simplified Weighted PageRank algorithm you need to implement is shown below. Please note that the formula to calculate PageRank values is slightly different to the one provided in the above paper.

![image](https://github.com/user-attachments/assets/cab39c8f-f5ad-4edf-9458-7c84c35c82f5)
![image](https://github.com/user-attachments/assets/cd9c277a-4c2f-4279-91b9-2f9cf705ea94)

Your program in pagerank.c must take three command-line arguments: d (damping factor), diffPR (sum of PageRank differences), maxIterations (maximum number of iterations), and using the algorithm described in this section, calculate the Weighted PageRank for every web page in the collection.

Here is an example of how the program is run:

./pagerank 0.85 0.00001 1000
Each of the expected output files in the provided files were generated using the above command.

Your program should output a list of URLs, one per line, to a file named pagerankList.txt, along with some information about each URL. Each line should begin with a URL, followed by its outdegree and Weighted PageRank (using the format string "%.7lf"). The values should be comma-separated, with a space after each comma. Lines should be sorted in descending order of Weighted PageRank. URLs with the same Weighted PageRank should be ordered alphabetically (ascending) by the URL.

Here is an example pagerankList.txt. Note that this was not generated from any of the provided test files, this is just an example to demonstrate the output format.

url31, 3, 0.0724132
url21, 1, 0.0501995
url11, 3, 0.0449056
url22, 4, 0.0360005
url34, 1, 0.0276197
url23, 3, 0.0234734
url32, 1, 0.0232762

We will use a tolerance of 10^−4 to check PageRank values when automarking. This means your answer for a PageRank value will be accepted if it is within 10^−4 of the expected value.

Assumptions/Constraints
All URLs in collection.txt are distinct.
URLs consist of alphanumeric characters only, and are at most 100 characters in length.
URLs do not necessarily start with url.
Web pages may have links to themselves and multiple links to the same web page. You must completely ignore self links and duplicate links, including when calculating the outdegree of each page.
There will be no missing web pages. This means each URL referenced in collection.txt or as an outlink in a web page will have a text file associated with it that is appropriately named. Also, every URL referenced as an outlink in a web page will be listed in collection.txt.
The provided command-line arguments will be valid. This means d will be a double between 0 and 1, diffPR will be a double greater than 0, and maxIterations will be a positive integer.
When we say "ordered alphabetically", we mean as determined by strcmp, which will compare the ASCII values of the characters.
All the text files required by the program will be in the current working directory when the program is executed. For Part 1, this includes the collection.txt and all the files containing the web pages.

Part 2 - Simple Search Engine

Your task is to implement a simple search engine in a file called searchPagerank.c that takes one or more search terms and outputs relevant URLs.

The program must make use of the data available in two files: invertedIndex.txt and pagerankList.txt. All other text files are not relevant for this task.

invertedIndex.txt contains data about what pages contain what words. Each line of the file contains one word followed by an alphabetically ordered list of URLs of pages that contain that word. Lines are ordered alphabetically by the initial word. Here is an example invertedIndex.txt:

design url2 url25 url31  
mars url101 url25 url31 
vegetation url31 url61
pagerankList.txt contains information about web pages including their URL, outdegree and Weighted PageRank. The format of this file is the same as that produced by your pagerank program in Part 1.

The program should take search terms as command-line arguments, find pages with one or more matching search terms and output the top 30 pages in descending order of the number of matching search terms to stdout. Pages with the same number of matching search terms should be ordered in descending order by their Weighted PageRank. Pages with the same number of matching search terms and the same Weighted PageRank should be sorted in increasing lexicographic order by URL.

Here is an example output. Note that this was not necessarily generated from any of the provided test files, this is just an example to demonstrate the output format.

./searchPagerank mars design
url31
url25
url2
url101
Assumptions/Constraints
Relevant assumptions from previous parts apply.
Search terms will be unique.
At least one search term will be given to the searchPagerank program.
Search terms will consist entirely of lowercase letters.
The maximum length of a line in invertedIndex.txt is 1000 characters.
Words in invertedIndex.txt will consist entirely of lowercase letters.
It is not guaranteed that all the search terms will be in invertedIndex.txt. Search terms that are not in invertedIndex.txt should be ignored, since there are no pages that contain them.
pagerankList.txt will contain correct URLs, outdegrees and PageRanks. If you do not produce the correct output in Part 1 then you should copy the expected result from pagerankList.exp into pagerankList.txt before testing.

Part 3 - Hybrid/Meta Search Engine using Rank Aggregation

Your task is to implement a program in scaledFootrule.c that combines search results (ranks) from multiple sources using Scaled Footrule Rank Aggregation, described below.

![image](https://github.com/user-attachments/assets/3f03feae-b0d6-4061-8447-54c8f260ebaa)

For example:
<img width="731" alt="scaled-footrule-example-1" src="https://github.com/user-attachments/assets/60098b35-95fb-4dd2-8ba4-1af291d1e8f5" />

The final ranking is derived by finding possible values of position 'P' such that the total scaled-footrule distance is minimum.

There are many different ways to assign possible values for 'P'. In the above example, P = [1, 3, 2, 5, 4]. Some other possible values are: P = [1, 2, 4, 3, 5], P = [5, 2, 1, 4, 3], P = [1, 2, 3, 4, 5], etc. When n = 5 (i.e., the number of distinct URLs is 5) there are 5! possible alternatives. When n = 10, there are 10! possible alternatives, which is is 3,628,800. A very simple and obviously inefficient approach could use brute-force search and generate all possible alternatives, calculate the total scaled-footrule distance for each alternative, and find the alternative with minimum scaled-footrule distance.

If you use such a brute-force search, you will receive a maximum of 65% for Part 3. However, if you implement a "smart" algorithm that avoids generating unnecessary alternatives, you can receive up to 100% for Part 3. You must document your algorithm such that your tutor can easily understand your logic, and clearly outline how you algorithm reduces the search space, otherwise you will not be awarded marks for your algorithm. Yes, it's only 35% of part-3 marks, but if you try it, you will find it very challenging and rewarding.

Note: You are not permitted to adapt or copy algorithms from the Internet for this task.

You need to write a program in scaledFootrule.c that aggregates ranks from files given as command-line arguments, and outputs an aggregated rank list with the minimum scaled footrule distance.

Example
Suppose that the file rankA.txt contains the following:

url1
url3
url5
url4
url2
And suppose that the file rankB.txt contains the following:

url3
url2
url1
url4
The following command will read ranks from files rankA.txt and rankB.txt and outputs minimum scaled footrule distance (using the format "%.6lf") on the first line, followed by the corresponding aggregated rank list.

./scaledFootrule rankA.txt rankB.txt
Two possible values for P with minimum distance are:

C = [url1, url2, url3, url4, url5]
P = [1, 4, 2, 5, 3]
P = [1, 5, 2, 4, 3]
Note that there may be multiple values of P that could result in the minimum scaled footrule distance. You only need to output one such list.

P = [1, 4, 2, 5, 3] corresponds to the following list:

1.400000
url1
url3
url5
url2
url4
P = [1, 5, 2, 4, 3] corresponds to the following list:

1.400000
url1
url3
url5
url4
url2
Your program should be able to handle more than two rank files, for example:

./scaledFootrule google.txt bing.txt yahoo.txt myRank.txt
Assumptions/Constraints
Relevant assumptions from previous parts apply.
All given rank files will be distinct.
Each ranking file will contain exactly one URL per line, and no other text. Furthermore, lines do not contain leading or trailing whitespace.

Testing
We have provided some test files for you to test your code for Part 1. Please note that you are expected to develop your own test cases for Parts 2 and 3.

Part 1
Here is an example of how to test Part 1.

assuming you are currently in your assignment 2 directorymake 

cd ex1
./pagerank 0.85 0.00001 1000

your program should have written its output to pagerankList.txt
now compare output in pagerankList.txt with expected output in pageRankList.exp
Note that you can test with other values of d, diffPR and maxIterations, but you will need to verify the output yourself, as there is no expected output for other values.

Part 2
Here is an example of how to test Part 2.

assuming you are currently in your assignment 2 directory make

cd ex1
./searchPagerank mars design
...

now verify the output yourself
Obviously, you can test with other search terms and other numbers of search terms, and you will also need to verify the output of this yourself.

Part 3
Here is an example of how to test Part 3.

assuming you are currently in your assignment 2 directory
assuming that rankA.txt and rankB.txt contain a list of URLs make

./scaledFootrule rankA.txt rankB.txt
...

now verify the output yourself
Note that if you create the rank files in your assignment 2 directory and not in any of the test directories (such as ex1), then you don't need to change into the test directories before running ./scaledFootrule.

You can also test with more rank files, for example:

assuming you are currently in your assignment 2 directory
assuming that rankA.txt, rankB.txt, rankC.txt and rankD.txt contain a list of URLs make

./scaledFootrule rankA.txt rankB.txt rankC.txt rankD.txt
...
now verify the output yourself
