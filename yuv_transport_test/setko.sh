rmmod netflowsample
rmmod kdrv_ai.ko
insmod ./lib/kdrv_ai.ko
insmod ./lib/kflow_cnn.ko
insmod ./lib/kflow_nue.ko
insmod ./lib/kflow_nue2.ko
insmod ./lib/kflow_cpu.ko
insmod ./lib/kflow_dsp.ko
insmod ./lib/kflow_ai_net.ko

