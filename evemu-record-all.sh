
rm -f /tmp/evemu-log-*

cd /dev/input/by-path/
for i in *; do
    if test ! -d "$i"; then
        sudo evemu-record "$i" 2>&1 > "/tmp/evemu-log-$i" &
        trap "kill $?" EXIT
    fi
done

tail -f /tmp/evemu-log-*
