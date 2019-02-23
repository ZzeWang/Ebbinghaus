# Ebbinghaus
 A small tool that can help you remember English words.


### 源何

Ebbinghaus是赫尔曼·艾宾浩斯的缩写，是他提出了记忆曲线的概念。Ebbinghaus就是基于记忆曲线而设计的记单词（如果你深入理解它的话，它不仅限于记单词）小程序。记单词最有效（也或许是最低效的方法）便是每天都把之前记忆过的单词拿出来再记一遍，当你尝试记忆十来个单词的时候，这样不失为一种好方法，但当你要记忆的单词量逐日扩大时，再把之前记过的单词都拿出来记一遍几乎是不可能完成的任务——这也是我为什么编写这个小程序的原因。

Ebbinghaus能够控制每个添加进入项目的项（一个基本单位），按照预定的历程进行循环。比如，在今天的课堂上，你新学习了“experience”这个单词，你想按照这样的次序记忆它：在学习了它之后的连续三天之内，你想每天都记它，之后每隔两天记忆一次，再之后每隔三天记忆一次，以此类推直到你能保证完全掌握了它

通过使一些你已经有印象的单词暂时“消失”从而使你把更多的经历放在新加入的单词上，这样就避免了单词逐日增多而无法把每个单词都记一次的尴尬

当日凡事都有例外，如果有一个单词你总是记不住，那么你可以使用Job提供一个frozen函数来暂时让它停止循环，使它每次都能够出现，然后当你记得差不多的时候再用free函数来让它继续进入循环

### 类以及结构

Ebbinghaus分为三个主要的类：**Job, Project,IO**.Project提供对每个不同的项目的管理。比如你创建了一个记单词的项目word，然后你又为了记住古诗词而由添加了一个项目poem，project提供管理这些项目的基本操作；Job提供每一个项目里每个项的管理操作。比如你向word项目中添加一个单词“first”，然后又向poem中添加了“沁园春雪”。除此之外，Job还提供了一层**项独立性**：显然，一个word项和一个poem项绝对不是同一类事物，例如，在word中，你可能仅仅存放一个单词即可，而在poem中，你可能需要存放一整段诗词。

**结构job\_info\_table\_base**和**结构job\_table\_item**提供了这层独立性。Job类只负责操作job\_table\_item。job\_table\_item仅有job\_id,staus等最基本的几个用于描述每个项属性的字段，而job\_info\_table\_base则记录了每个不同项的不同之处。它是一个基类，每个不同的job\_info类都应该继承它，然后编写你自己的job info

![](https://raw.githubusercontent.com/TongChuanLiudehua/Ebbinghaus/master/dir/ebb/cengci.PNG)

我在Job.h中已经编写了两个job info类，一个是用于记忆单词的job\_info\_item，一个是job\_info\_for\_net(用于记忆计算机网络中名词的缩写和全拼)

IO类提供存储独立性，我是基于文件系统而编写的，如果你想使用数据库，那么修改IO即可


### 基本用法

This tool can be compiled by any c++ compiler, so you can use your only c++ compiler compiles the source in **CMakeList.txt**

#### create a project

        --pa [name]

#### delete a project
        --pd [name]

#### change the default project

        --pc [name]

#### get help infomation

        --help

#### add a job into the default project

        --add [job name]

or add multiple jobs at once 

        --muladd [job, ...]
#### frozen a job
		--frozen [job]
#### free a job
		--free [job]
#### delete a job from your prject
		--remove [job]

### 示例

1. 创建Word项目

![](https://raw.githubusercontent.com/TongChuanLiudehua/Ebbinghaus/master/dir/ebb/createpro.PNG)

2. 创建另一个项目other

![](https://raw.githubusercontent.com/TongChuanLiudehua/Ebbinghaus/master/dir/ebb/createproother.PNG)

3. 切换至Word项目

![](https://raw.githubusercontent.com/TongChuanLiudehua/Ebbinghaus/master/dir/ebb/changepro.PNG)

4. 向Word项目中添加一个项frist（单词）

![](https://raw.githubusercontent.com/TongChuanLiudehua/Ebbinghaus/master/dir/ebb/add.PNG)

5. 启动Word

![](https://raw.githubusercontent.com/TongChuanLiudehua/Ebbinghaus/master/dir/ebb/start.PNG)

6. 查看Word

![](https://raw.githubusercontent.com/TongChuanLiudehua/Ebbinghaus/master/dir/ebb/look.PNG)

![](https://raw.githubusercontent.com/TongChuanLiudehua/Ebbinghaus/master/dir/ebb/look2.PNG)

7. 结束Word

![](https://raw.githubusercontent.com/TongChuanLiudehua/Ebbinghaus/master/dir/ebb/done.PNG)

8. 一次添加多个项

![](https://raw.githubusercontent.com/TongChuanLiudehua/Ebbinghaus/master/dir/ebb/muladd.PNG)

9. 组合命令

![](https://raw.githubusercontent.com/TongChuanLiudehua/Ebbinghaus/master/dir/ebb/组合.PNG)

10. 暂停项

![](https://raw.githubusercontent.com/TongChuanLiudehua/Ebbinghaus/master/dir/ebb/frozen.PNG)

11. 恢复项

![](https://raw.githubusercontent.com/TongChuanLiudehua/Ebbinghaus/master/dir/ebb/free.PNG)

12. 删除项

![](https://raw.githubusercontent.com/TongChuanLiudehua/Ebbinghaus/master/dir/ebb/remove.PNG)

13. 删除Word项目

![](https://raw.githubusercontent.com/TongChuanLiudehua/Ebbinghaus/master/dir/ebb/deletepro.PNG)

## 后记

Ebbinghaus所用的命令行解析程序是我编写的[Cflag](https://github.com/TongChuanLiudehua/Cflag),如果感兴趣欢迎浏览
