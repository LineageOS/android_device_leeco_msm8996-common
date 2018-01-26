#
# System Properties for msm8996-common
#

# system props for the MM modules
PRODUCT_PROPERTY_OVERRIDES += \
    audio.deep_buffer.media=true \
    media.aac_51_output_enabled=true \
    media.stagefright.enable-player=true \
    media.stagefright.enable-http=true \
    media.stagefright.enable-aac=true \
    media.stagefright.enable-qcp=true \
    media.stagefright.enable-fma2dp=true \
    media.stagefright.enable-scan=true \
    mmp.enable.3g2=true \
    mm.enable.smoothstreaming=true \
    mm.enable.qcom_parser=4194303 \
    persist.mm.enable.prefetch=true \
    persist.media.treble_omx=false

# Enable AAC 5.1 output
PRODUCT_PROPERTY_OVERRIDES += \
    media.aac_51_output_enabled=true

# Additional i/p buffer in case of encoder DCVS
PRODUCT_PROPERTY_OVERRIDES += \
    vidc.enc.dcvs.extra-buff-count=2

# Set default power mode to low power for encoder
PRODUCT_PROPERTY_OVERRIDES += \
    vidc.debug.perf.mode=2

# Audio
PRODUCT_PROPERTY_OVERRIDES += \
    persist.audio.ssr.3mic=false \
    af.fast_track_multiplier=1 \
    audio.heap.size.multiplier=7 \
    ro.vendor.audio.sdk.fluencetype=fluence \
    ro.vendor.audio.sdk.ssr=false \
    vendor.audio_hal.period_size=192 \
    vendor.audio.tunnel.encode=false \
    vendor.voice.path.for.pcm.voip=true \
    persist.vendor.audio.fluence.voicecall=true \
    persist.vendor.audio.fluence.voicerec=true \
    persist.vendor.audio.fluence.audiorec=false \
    persist.vendor.audio.fluence.speaker=true \
    vendor.audio.offload.buffer.size.kb=64 \
    vendor.audio.offload.video=true \
    vendor.audio.offload.pcm.16bit.enable=true \
    vendor.audio.offload.pcm.24bit.enable=true \
    vendor.audio.offload.track.enable=false \
    vendor.audio.offload.multiaac.enable=true \
    vendor.audio.offload.gapless.enabled=true \
    vendor.audio.offload.passthrough=false \
    vendor.audio.offload.multiple.enabled=true \
    vendor.audio.offload.min.duration.secs=30 \
    vendor.audio.safx.pbe.enabled=true \
    vendor.audio.parser.ip.buffer.size=262144 \
    vendor.audio.dolby.ds2.enabled=false \
    vendor.audio.dolby.ds2.hardbypass=false

# Bluetooth
PRODUCT_PROPERTY_OVERRIDES += \
    bt.max.hfpclient.connections=1 \
    qcom.bluetooth.soc=rome \
    ro.bluetooth.wipower=true \
    ro.bluetooth.emb_wp_mode=true

# System property for cabl
PRODUCT_PROPERTY_OVERRIDES += \
    ro.qualcomm.cabl=2

# Property for vendor specific library
PRODUCT_PROPERTY_OVERRIDES += \
    ro.vendor.gt_library=libqti-gt.so \
    ro.vendor.at_library=libqti-at.so \
    sys.games.gt.prof=1

# Camera
PRODUCT_PROPERTY_OVERRIDES += \
    persist.camera.HAL3.enabled=1 \
    persist.camera.imglib.fddsp=1 \
    persist.camera.llc=1 \
    persist.camera.llnoise=1 \

# Display power reduction (FOSS)
PRODUCT_PROPERTY_OVERRIDES += \
    ro.qcom.dpps.sensortype=3 \
    ro.qualcomm.display.paneltype=1 \
    ro.qualcomm.foss=1 \
    config.foss.xml=1 \
    config.foss.path="/system/etc/FOSSConfig.xml"

# CNE
PRODUCT_PROPERTY_OVERRIDES += \
    persist.cne.feature=1

# Data modules
PRODUCT_PROPERTY_OVERRIDES += \
    ro.use_data_netmgrd=true \
    persist.data.netmgrd.qos.enable=true \
    persist.data.mode=concurrent

# GPS
PRODUCT_PROPERTY_OVERRIDES += \
    persist.gps.qc_nlp_in_use=0 \
    ro.gps.agps_provider=1

# Graphics
PRODUCT_PROPERTY_OVERRIDES += \
    debug.sf.hw=1 \
    debug.egl.hw=1 \
    debug.gralloc.enable_fb_ubwc=1 \
    dev.pm.dyn_samplingrate=1 \
    persist.demo.hdmirotationlock=false \
    persist.debug.wfd.enable=1 \
    persist.sys.wfd.virtual=0 \
    sdm.perf_hint_window=50 \
    persist.hwc.enable_vds=1 \
    sdm.debug.disable_rotator_split=1 \
    ro.persist.qcapb=1

# OpenGLES
PRODUCT_PROPERTY_OVERRIDES += \
    ro.opengles.version=196610

#enable Apical AD
PRODUCT_PROPERTY_OVERRIDES += \
    ro.qcom.ad=1 \
    ro.qcom.ad.sensortype=3

# Perf
PRODUCT_PROPERTY_OVERRIDES += \
    ro.am.reschedule_service=true \
    ro.vendor.extension_library=libqti-perfd-client.so \
    ro.min_freq_0=307200 \
    ro.min_freq_4=307200 \
    ro.sys.fw.bg_apps_limit=60

# QCOM
PRODUCT_PROPERTY_OVERRIDES += \
    ro.frp.pst="/dev/block/bootdevice/by-name/frp" \
    drm.service.enabled=true

# RIL
PRODUCT_PROPERTY_OVERRIDES += \
    rild.libpath="/vendor/lib64/libril-qc-qmi-1.so" \
    persist.rild.nitz_plmn="" \
    persist.rild.nitz_long_ons_0="" \
    persist.rild.nitz_long_ons_1="" \
    persist.rild.nitz_long_ons_2="" \
    persist.rild.nitz_long_ons_3="" \
    persist.rild.nitz_short_ons_0="" \
    persist.rild.nitz_short_ons_1="" \
    persist.rild.nitz_short_ons_2="" \
    persist.rild.nitz_short_ons_3="" \
    ril.subscription.types=NV,RUIM \
    DEVICE_PROVISIONED=1 \
    persist.volte_enalbed_by_hw=1 \
    persist.radio.data_ltd_sys_ind=1 \
    ro.telephony.default_network=10,10 \
    telephony.lteOnCdmaDevice=1 \
    ro.telephony.call_ring.multiple=false \
    persist.radio.apm_sim_not_pwdn=1 \
    persist.radio.custom_ecc=1 \
    persist.radio.sib16_support=1 \
    persist.data.qmi.adb_logmask=0 \
    persist.net.doxlat=true \
    persist.oem.dump=0 \
    persist.radio.hw_mbn_update=0 \
    persist.radio.sw_mbn_update=0 \
    persist.radio.start_ota_daemon=0 \
    persist.data.iwlan.enable=true \
    persist.radio.VT_ENABLE=1 \
    persist.radio.REVERSE_QMI=0 \
    persist.radio.ROTATION_ENABLE=1 \
    persist.dbg.volte_avail_ovr=1 \
    persist.dbg.vt_avail_ovr=1

#default SAR mode 0:off/1:on
PRODUCT_PROPERTY_OVERRIDES += \
    persist.sys.sar_mode=1

PRODUCT_PROPERTY_OVERRIDES += \
    persist.radio.calls.on.ims=true \
    persist.radio.jbims=true \
    persist.rcs.supported=1 \
    persist.radio.domain.ps=false \
    persist.radio.csvt.enabled=false \
    persist.vendor.radio.cs_srv_type=0 \
    persist.vendor.radio.rat_on=combine \
    persist.vendor.radio.mt_sms_ack=20 \
    persist.vendor.radio.ignore_dom_time=5 \
    persist.vendor.radio.force_on_dc=true \
    persist.vendor.radio.facnotsup_as_nonw=1

# RmNet Data
PRODUCT_PROPERTY_OVERRIDES += \
    persist.rmnet.data.enable=true \
    persist.data.wda.enable=true \
    persist.data.df.dl_mode=5 \
    persist.data.df.ul_mode=5 \
    persist.data.df.agg.dl_pkt=10 \
    persist.data.df.agg.dl_size=4096 \
    persist.data.df.mux_count=8 \
    persist.data.df.iwlan_mux=9 \
    persist.data.df.dev_name=rmnet_usb0

# Timeservice
PRODUCT_PROPERTY_OVERRIDES += \
    persist.timed.enable=true

# Wifi
PRODUCT_PROPERTY_OVERRIDES += \
    wifi.interface=wlan0

# SSR
PRODUCT_PROPERTY_OVERRIDES += \
    persist.sys.ssr.restart_level=ALL_ENABLE

# Fastcharge
PRODUCT_PROPERTY_OVERRIDES += \
    persist.sys.le_fast_chrg_enable=1
