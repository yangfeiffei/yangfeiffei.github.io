#!/bin/bash 

if [ `grep "NEVER" custom/css/main.css|wc -l` != 0 ];
then
	echo hhhhhhhhh;
	mv custom/css/main.css custom/css/main.css.bak;
	sed '/NEVER/,$d' custom/css/main.css.bak > custom/css/main.css;
fi
echo ""
echo "/* NEVER Delete it, add by scrips */ 
$(ls _posts |grep 2019 |awk -F "-" '{ print "#"$1"-"$2"-"$3}' | xargs | sed 's/ /,/g') {  
fill: green; 
}
/* SCRIPTS add css END */
" >> custom/css/main.css


