#!/vendor/bin/sh

if [ ! -f /data/data/com.uei.quicksetsdk.letv/files/Settings ]; then

	# make the required directories
	mkdir -p /data/data/com.uei.quicksetsdk.letv/files

	# Copy Settings file
	cp /vendor/etc/UEISettings /data/data/com.uei.quicksetsdk.letv/files/Settings

	chmod 751 /data/data/com.uei.quicksetsdk.letv/
	chmod 777 /data/data/com.uei.quicksetsdk.letv/files
	chmod 644 /data/data/com.uei.quicksetsdk.letv/files/Settings

	UEIUSER=$(stat -c '%U' /data/data/com.uei.quicksetsdk.letv/)

	if [ -n "$UEIUSER" ]; then
		chown -R "$UEIUSER":"$UEIUSER" /data/data/com.uei.quicksetsdk.letv/
		restorecon -R /data/data/com.uei.quicksetsdk.letv
	fi
fi