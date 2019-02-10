#Ebbinghaus
 A small tool that can help you remember English words.


###Why


Ebbinghaus is a approch that can repeat a word or a group words by routine process.For example, today, you learn a word "experiment" at your classroom, and you want to repeatly recall this word until you can guarantee that you have remembered the word.The easliest way to repeat a word or a list of word is to read or write them everyday, however, it is impossible to do so when you aim to remember a heap of words, such as a dictionary(It is why I decide to develop this tool).The situation becomes worse when the number of words rises day by day.


###Conception

The small tool contains three classes, **Job, Proejct and IO**.You can reweite IO so that it can use database rathe than file system. The Project can only use file system to store some necessary settings.Job support the main function in this tool.

Job is a class template.The default template parameter is **job\_info\_table\_item** that implement the basic useage of remembering word.

You can implement your own job item by inheriting the base class **job\_info\_item\_base**.


###Usage

This tool can be compiled by any c++ compiler, so you can use your only c++ compiler compiles the source in **CMakeList.txt**

####create a project

	--pa [name]

####delete a project
	--pd [name]

####change the default project

	--pc [name]

####get help infomation

	--help

####add a job into the default project

	--add+ [job name]

or add multiple jobs at once 

	--adds [job, ...]
