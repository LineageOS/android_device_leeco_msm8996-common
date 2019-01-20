#!/vendor/bin/sh

if [ ! -f /data/data/com.uei.quicksetsdk.letv/files/Settings ]; then

	# make the required directories
	mkdir -p /data/data/com.uei.quicksetsdk.letv/files

	# Copy Settings file
	cp /vendor/etc/UEISettings /data/data/com.uei.quicksetsdk.letv/files/Settings

	chmod 751 /data/data/com.uei.quicksetsdk.letv/
	chmod 777 /data/data/com.uei.quicksetsdk.letv/files
	chmod 644 /data/data/com.uei.quicksetsdk.letv/files/Settings

	chown -R u0_a32:u0_a32 /data/data/com.uei.quicksetsdk.letv/
	restorecon_recursive /data/data/com.uei.quicksetsdk.letv
fi