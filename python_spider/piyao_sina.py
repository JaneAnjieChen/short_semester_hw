from selenium import webdriver
from selenium.webdriver.common.keys import Keys  #keyboard
from selenium.webdriver.chrome.options import Options
from selenium.webdriver.common.action_chains import ActionChains  #mouse
import requests
import time
import os
import re
import platform
from lxml import etree
from datetime import datetime

url='http://piyao.sina.cn'
if platform.system()=='Windows':
    chrome_diver_path="chromedriver.exe"
elif platform.system()=='Linux'or platform.system()=='Darwin':
    chrome_diver_path="./chromedriver"
else:
    print('Unknown system type, quit...')

chrome_options = Options()
#whether show window
chrome_options.add_argument('--headless')
chrome_options.add_argument('--disable-gpu')
driver = webdriver.Chrome(chrome_options=chrome_options, 
                                executable_path=chrome_diver_path)
# driver.get(url)

#variables
titles=[]
dates=[]
titles_dates=[]
likes=[]
new_likes=[]
titles_datesAndlikes=[]
high_like=[]

#加载界面
driver.get("http://piyao.sina.cn")
time.sleep(3)

#滚动条置底bit by bit,repeats
for loop in range(0,10):
    loop+=1

    driver.execute_script('window.scrollTo(0, document.body.scrollHeight)')
    time.sleep(4)

    date=driver.find_elements_by_xpath('//div[@class="zy_day"]/div[@class="day_date"]')
    # for d in date:
    #     print(d.text,'\n')
    for i,e in enumerate(date):
        j=i+1

        title=driver.find_elements_by_xpath('//div[@class="zy_day" and position()='+str(j)+']/div[@class="day_date"]/following-sibling::ul//div[@class="left_title"]')
        for t in title:
            titles.append(t.text)
            tmp=t.text+e.text
            titles_dates.append(tmp)

        like=driver.find_elements_by_xpath('//div[@class="zy_day" and position()='+str(j)+']/div[@class="day_date"]/following-sibling::ul//div[@class="left_btns"]/div[@class="like_text"]')
        for l in like:
            #t=int(l.text)
            likes.append(l.text)
           
        date=driver.find_elements_by_xpath('//div[@class="zy_day" and position()='+str(j)+']/div[@class="day_date"]')
        for d in date:
            dates.append(d.text)

counter=0
for l in likes:
    temp=titles_dates[counter]+'   '+l
    titles_datesAndlikes.append(temp)#拼接
    counter+=1

titles_datesAndlikes=list(set(titles_datesAndlikes))#去重
#print(titles_datesAndlikes)

for tdl in titles_datesAndlikes:
    new_like=int(str(tdl.split("   ")[1]))
    new_likes.append(new_like)#将去重后的likes放入new_likes

titles_datesAndlikes=zip(titles_datesAndlikes,new_likes)

high_like=sorted(titles_datesAndlikes,key=lambda x:x[1])#根据new_likes排序
print(high_like)
#print(high_like)
for x in high_like[-10:]:
    print(x[0], '\t', x[1])

