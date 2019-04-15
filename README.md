# biterm-topic-model
重构论文A Biterm Topic Model for Short Texts提供的源代码，编译成一个python 扩展模块

编译：
make 
如果是windows平台，需要小修改

安装:

python setup.py install

使用 demo:

from biterm.Model import bitermModel as model   

mymodel=Model(20,99509,2.5,0.01,5,101)    #topic、voc size、alpha、belta、savestep、iteration

mymodel.train("data/data.txt")    

relist=mymodel.predict([232,435,56,76,890])    #words indexs

mymodel.savemodel("save/")    

del mymodel   

mymodel=Model(20,99509,2.5,0.01,5,101)   

mymodel.loadmodel("save/")   

relist=mymodel.predict([232,435,56,76,890])
