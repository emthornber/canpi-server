#! /bin/bash
################################################################################
#
#	canpiserverctl.bash
#
#   Script to configure avahi-daemon (bonjour) from canpi.cfg and control canpid
#
#   03 February, 2024 - E M Thornber
#   Created
#
################################################################################

# Read configuration
if test -f $CANPI_INI_FILE ; then
    # Configuration file exists - source it
    . $CANPI_INI_FILE
else
    # Set bonjour defaults
    service_name=`uname -n`
    tcpport="5555"
    edserver="Y"
fi
ED_FILE=${CPSRV_HOME}/ed.enabled

setup_bonjour() {
    # Change the service name and port
    AVAHI_FILE=multiple.service
    BONJOUR_TEMPLATE=$CPSVR_HOME/${AVAHI_FILE}.in
    BONJOUR_FILE=/etc/avahi/services/${AVAHI_FILE}
    sed -e "s/|SERVICENAME|/$service_name/" -e "s/|PORT|/$tcpport/" $BONJOUR_TEMPLATE > $BONJOUR_FILE

    # Restart the service
    systemctl restart avahi-daemon
    sleep 1
    echo Bonjour service is `systemctl is-active avahi-daemon`
}

start_canpid() {
    if [ `pgrep canpid` ] ;
    then
	echo canpi already running
    else
	echo starting canpid
	/usr/local/bin/canpid >> "/var/log/canpid/stdout.log" 2>>"/var/log/canpid/stderr.log"
	if [ ! `pgrep canpid` ] ; then echo canpid did not start ; fi
    fi

    # Check if edserver is enabled
    if [ ${edserver,,} == "y" ] ; then
	touch $ED_FILE
    else
	rm -f $ED_FILE
    fi
}

stop_canpid() {
    if [ `pgrep canpid` ]; then
	pkill canpid
	echo canpid killed
    else
	echo canpid not active
    fi
    rm -f $ED_FILE
}

# main code
case "$1" in
start)
    setup_bonjour
    start_canpid
    systemctl restart avahi-daemon
    ;;
restart)
    $0 stop
    $0 start
    ;;
stop)
    stop_canpid
    systemctl restart avahi-daemon
    ;;
*)
    echo "Usage: $0 (start|restart|stop)"
    exit 1
    ;;
esac

exit 0

