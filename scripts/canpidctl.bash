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
#   15 February, 2024 - E M Thornber
#   Remove use of ed.enabled flag file
#
################################################################################

# Read configuration
if test -f "$CANPI_INI_FILE" ; then
    # Configuration file exists - source it
    . $CANPI_INI_FILE
else
    # Set bonjour defaults
    service_name=`uname -n`
    tcpport="5555"
    edserver="Y"
fi
AVAHI_FILE=multiple.service
BONJOUR_TEMPLATE=${CPSRV_HOME}/${AVAHI_FILE}.in
BONJOUR_FILE=/etc/avahi/services/${AVAHI_FILE}
PID_FILE=/run/canpid.pid

setup_bonjour() {
    # Change the service name and port
    sed -e "s/|SERVICENAME|/$service_name/" -e "s/|PORT|/$tcpport/" $BONJOUR_TEMPLATE > $BONJOUR_FILE

    # Restart the service
    systemctl restart avahi-daemon
}

teardown_bonjour() {
    rm -f $BONJOUR_FILE

    # Restart the service
    systemctl restart avahi-daemon
}

start_canpid() {
    pid=`pgrep --exact canpid`
    if [ $? -eq 0 ] ;
    then
	echo canpi already running
    else
	echo starting canpid
	/usr/local/bin/canpid >> "/var/log/canpid/stdout.log" 2>>"/var/log/canpid/stderr.log" &
	echo $! > $PID_FILE
	if [ ! `pgrep --exact canpid` ] ; then echo canpid did not start ; fi
    fi

    # Check if edserver is enabled
    if [ ${edserver,,} == "y" ] ; then
	setup_bonjour
    else
	teardown_bonjour
    fi
}

stop_canpid() {
    if [ `pgrep --exact canpid` ]; then
	pkill -9 --exact canpid
	echo canpid killed
    else
	echo canpid not active
    fi
    teardown_bonjour
}

# main code
case "$1" in
start)
    start_canpid
    ;;
restart)
    $0 stop
    $0 start
    ;;
stop)
    stop_canpid
    ;;
*)
    echo "Usage: $0 (start|restart|stop)"
    exit 1
    ;;
esac

exit 0

