#!/bin/sh
WORKDIR=$PWD/work
TARGET=vpndatalist.html
DEV=$WORKDIR/dev
SWAP=$WORKDIR/swap.html
POLL=$WORKDIR/poll.dat
rm -rf $WORKDIR
mkdir -p $WORKDIR

function substring()
{
	getd=$(cat $DEV| grep $1)
	echo ${getd##$1=}
}

for ((net=1;net<2;++net))
	do
		for ((dev=0;dev<30;++dev))
			do
			IP=192.168.$net.$dev
			echo Work for : $IP
			ping $IP -n 1 -w 300 >>/dev/NULL
			if [ $? -ne 1 ]; then
			curl --connect-timeout 3 http://$IP/device_serial.html > $DEV
			XIP=$(substring "DEVICE_XIP")
			SN=$(echo "$(substring "SERIAL")" | tr -d " -")
			VPN_NAME=$(substring "DEVICE_ACCOUNT")
			VPN_PASS=$(substring "DEVICE_ACCPASSWD")
			echo "{\"VPN_SN\":\"$SN\",\"IP\":\"$XIP\",\"VPN_NAME\":\"$VPN_NAME\",\"VPN_PASS\",\"$VPN_PASS\"}">>$POLL
			fi
		done
done

cat $POLL | grep -v "{\"VPN_SN\":\"\",\"IP\":\"\",\"VPN_NAME\":\"\",\"VPN_PASS\",\"\"}" >$POLL.old
cp $POLL.old $POLL
rm $POLL.old

cat >$SWAP<<EOF
json:{
"total":"$(cat $POLL | wc -l)"
"rows":[
$(cat $POLL)
]
}

EOF

cat $SWAP | sed s/[[:space:]]//g > $TARGET