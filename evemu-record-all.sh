
cd /dev/input/by-path/
for i in *; do
    sudo evemu-record "$i" 2>&1 > "/tmp/evemu-log-$i" &
    trap "kill $?" EXIT
done

tail -f /tmp/evemu-log-*
