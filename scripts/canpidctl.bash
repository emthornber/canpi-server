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
#   26 November, 2026 - K Bruce
#   Read canpi.cfg without running it as shell commands, and check the values
#   used before passing them to sed and the bonjour file
#
################################################################################

# Read configuration
#
# canpi.cfg is not sourced: it is written by canpid and the configuration web
# app, and a value must never run as a command here. Each value is read as
# text, and a missing or unsuitable value falls back to the default.

# cfg_value KEY: print the value of the first KEY=... line in $CANPI_INI_FILE
# (the one canpid uses), without surrounding spaces or double quotes
cfg_value() {
    local line value=""
    [ -f "$CANPI_INI_FILE" ] || return 0
    while IFS= read -r line || [ -n "$line" ] ; do
        line=${line%$'\r'}
        if [[ $line =~ ^[[:space:]]*$1[[:space:]]*=(.*)$ ]] ; then
            value=${BASH_REMATCH[1]}
            value=${value#"${value%%[![:space:]]*}"}
            value=${value%"${value##*[![:space:]]}"}
            if [[ $value == \"*\" ]] ; then value=${value:1:${#value}-2} ; fi
            break
        fi
    done < "$CANPI_INI_FILE"
    printf '%s' "$value"
}

service_name=$(cfg_value service_name)
tcpport=$(cfg_value tcpport)
edserver=$(cfg_value edserver)

# The service name goes into a sed replacement and the bonjour file
if [[ ! $service_name =~ ^[A-Za-z0-9._\ -]{1,63}$ ]] ; then
    [ -n "$service_name" ] && echo "service_name '$service_name' not usable, using `uname -n`" >&2
    service_name=`uname -n`
fi
if [[ ! $tcpport =~ ^[0-9]{1,5}$ ]] || (( 10#$tcpport < 1 || 10#$tcpport > 65535 )) ; then
    [ -n "$tcpport" ] && echo "tcpport '$tcpport' not usable, using 5555" >&2
    tcpport="5555"
fi
[ -n "$edserver" ] || edserver="Y"

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
    if [ "${edserver,,}" == "y" ] ; then
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

