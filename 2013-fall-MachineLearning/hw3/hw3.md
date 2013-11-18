Machine Learning Homework #3
============================
Qing-Cheng Li 

R01922024

#1. C

a. 0.01 * (1-(8+1)/10) = 0.001

b. 0.01 * (1-(8+1)/25) = 0.0064

c. 0.01 * (1-(8+1)/100) = 0.0091

d. 0.01 * (1-(8+1)/500) = 0.00982

-- select (c).

#2. 

*Jizz*

#3. A,B

<img src="http://linux.qcl.tw/html3/IMAG2962.jpg" alt="p8" style="width: 600px;"/>

#4. D

(a)(b)在 $yw^Tx=1$ 有一個轉折不連續，不可微分；(c)在0亦有轉折不連續，不可微分。(d)連續可微。

#5. D

When $w^Tx$ is large enough, (d) is like PLA. (accroding to slide #11 page 11)

#6.

$E(u,v)=e^u+e^{2v}+e^{uv}+u^2-2uv+2v^2-3u-2v$

$\bigtriangledown E(u,v) = (\frac{\partial E}{\partial u},\frac{\partial E}{\partial v})$

$\frac{\partial E}{\partial u} = e^{u}+ve^{uv}+2u-2v-3$

$\frac{\partial E}{\partial v} = 2e^{2v}+ue^{uv}-2u+4v-2$

$\bigtriangledown E(u,v) = (-2,0)$

#7.

> $ python hw3p7.py

1. (-1.931353352832366, -0.02) 
2. (-1.8643679281636854,-0.036968701451556445)
3. (-1.7989738194344052,-0.05137792829437576)
4. (-1.7351130572126412,-0.06361748194580197)
5. (-1.6727365278776065,-0.07400946260992759)

#8. 3/8/-1/-2/0/3

<img src="http://linux.qcl.tw/html3/IMAG2961.jpg" alt="p8" style="width: 600px;"/>

#9. A

$\bigtriangledown E + \bigtriangledown^2 E = 0\ p \\ p = -(\bigtriangledown^2 E)^{-1}\bigtriangledown E $

select (a).

#10.

> $ python hw3p10.py

1. (0.07692307692307693, 0.0)
2. (0.143561753527925, 0.0029023140091422642)
3. (0.20120879953531765, 0.007341944796713773)
4. (0.25110373042431666, 0.012467030623187523)
5. (0.294360105434023, 0.017763043039507893)

#11. A

Can not let $x_1$ and $x_3$ be the same side but $x_2$ and $x_4$ be another side, so (b),(c),(d) can not be shattered. -- select (a).

#12. 

*Jizz*

#13. A

> $ python hw3p13.py

err avg = 0.186798
close to (a)

#14. A

> $ python hw3p14.py

#15. C

> $ python hw3p14.py

#16.

*JIZZ*

#17.

*JIZZ*

#18. 0.477

> $ python hw3p18.py

#19. 0.477

> $ python hw3p19.py

#20. 0.477

> $ python hw3p20.py


  [1]: http://linux.qcl.tw/html3/IMAG2961.jpg