/*
 * ASoC driver for MikroElectronika 506 (wm8731 proto board)
 * connected to a RockPi 4
 *
 * Author: Chris Chronopoulos, <chronopoulos.chris@gmail.com>
 *	       Copyright 2020

 * Based on rpi-proto.c by Florian Meier
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/platform_device.h>

#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/soc.h>
#include <sound/jack.h>

#include "wm8731.h"

static const unsigned int wm8731_rates_12288000[] = {
	8000, 32000, 48000, 96000,
};

static struct snd_pcm_hw_constraint_list wm8731_constraints_12288000 = {
	.list = wm8731_rates_12288000,
	.count = ARRAY_SIZE(wm8731_rates_12288000),
};

static int snd_mikroe_506_startup(struct snd_pcm_substream *substream)
{
	/* Setup constraints, because there is a 12.288 MHz XTAL on the board */
	snd_pcm_hw_constraint_list(substream->runtime, 0,
				SNDRV_PCM_HW_PARAM_RATE,
				&wm8731_constraints_12288000);
	return 0;
}

static int snd_mikroe_506_hw_params(struct snd_pcm_substream *substream,
				       struct snd_pcm_hw_params *params)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_dai *codec_dai = rtd->codec_dai;
	int sysclk = 12288000; /* This is fixed on this board */

    int ret;

	/* Set proto sysclk */
	ret = snd_soc_dai_set_sysclk(codec_dai, WM8731_SYSCLK_XTAL,
			sysclk, SND_SOC_CLOCK_IN);
	if (ret < 0) {
		dev_err(rtd->card->dev,
				"Failed to set WM8731 SYSCLK: %d\n", ret);
		return ret;
	}

	return 0;
}

/* machine stream operations */
static struct snd_soc_ops snd_mikroe_506_ops = {
	.startup = snd_mikroe_506_startup,
	.hw_params = snd_mikroe_506_hw_params,
};

static struct snd_soc_dai_link snd_mikroe_506_dai[] = {
{
	.name		= "WM8731",
	.stream_name	= "WM8731 HiFi",
	.cpu_dai_name	= "rockchip-i2s.1",
	.codec_dai_name	= "wm8731-hifi",
	.platform_name	= "rockchip-i2s.1",
	.codec_name	= "wm8731.7-001a",
	.dai_fmt	= SND_SOC_DAIFMT_I2S
				| SND_SOC_DAIFMT_NB_NF
				| SND_SOC_DAIFMT_CBM_CFM,
	.ops		= &snd_mikroe_506_ops,
},
};

/* audio machine driver */
static struct snd_soc_card snd_mikroe_506 = {
	.name		= "snd_mikroe_506",
	.owner		= THIS_MODULE,
	.dai_link	= snd_mikroe_506_dai,
	.num_links	= ARRAY_SIZE(snd_mikroe_506_dai),
};

static int snd_mikroe_506_probe(struct platform_device *pdev)
{
	int ret = 0;

	snd_mikroe_506.dev = &pdev->dev;

	if (pdev->dev.of_node) {
		struct device_node *i2s_node;
		struct snd_soc_dai_link *dai = &snd_mikroe_506_dai[0];
		i2s_node = of_parse_phandle(pdev->dev.of_node,
				            "i2s-controller", 0);

		if (i2s_node) {
			dai->cpu_dai_name = NULL;
			dai->cpu_of_node = i2s_node;
			dai->platform_name = NULL;
			dai->platform_of_node = i2s_node;
		}
	}

	ret = devm_snd_soc_register_card(&pdev->dev, &snd_mikroe_506);
	if (ret && ret != -EPROBE_DEFER)
		dev_err(&pdev->dev,
				"snd_soc_register_card() failed: %d\n", ret);

	return ret;
}

static const struct of_device_id snd_mikroe_506_of_match[] = {
	{ .compatible = "mikroe,506", },
	{},
};
MODULE_DEVICE_TABLE(of, snd_mikroe_506_of_match);

static struct platform_driver snd_mikroe_506_driver = {
	.driver = {
		.name   = "snd-mikroe-506",
		.owner  = THIS_MODULE,
		.of_match_table = snd_mikroe_506_of_match,
	},
	.probe	  = snd_mikroe_506_probe,
};

module_platform_driver(snd_mikroe_506_driver);

MODULE_AUTHOR("Chris Chronopoulos");
MODULE_DESCRIPTION("ASoC Driver for Rock Pi connected to MikroElectronika 506");
MODULE_LICENSE("GPL");
