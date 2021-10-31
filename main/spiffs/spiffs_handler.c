/*
 * vfs_handler.c
 *
 *  Created on: Oct 19, 2021
 *      Author: kai
 */
#include "spiffs_handler.h"
#include <stdio.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include <esp_log.h>
#include <esp_err.h>
#include <esp_spiffs.h>
#include "bsp.h"
#include "lsm6dsox_defines.h"
#include "lsm6dsox_config.h"


static const char* TAG = "SPIFFS Handler";
static esp_vfs_spiffs_conf_t conf;

static unsigned char spiffs_handler_get_hex_value(const char letter);

void spiffs_handler_register()
{
   ESP_LOGI(TAG, "Initializing SPIFFS");

   conf.base_path = "/spiffs";
   conf.partition_label = NULL;
   conf.max_files = 5;
   conf.format_if_mount_failed = true;

   // Use settings defined above to initialize and mount SPIFFS filesystem.
   // Note: esp_vfs_spiffs_register is an all-in-one convenience function.
   esp_err_t ret = esp_vfs_spiffs_register(&conf);

   if(ret != ESP_OK)
   {
      if(ret == ESP_FAIL)
      {
         ESP_LOGE(TAG, "Failed to mount or format filesystem");
      }
      else if(ret == ESP_ERR_NOT_FOUND)
      {
         ESP_LOGE(TAG, "Failed to find SPIFFS partition");
      }
      else
      {
         ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
      }
   }

   size_t total = 0, used = 0;
   ret = esp_spiffs_info(conf.partition_label, &total, &used);
   if (ret != ESP_OK) {
       ESP_LOGE(TAG, "Failed to get SPIFFS partition information (%s)", esp_err_to_name(ret));
   } else {
       ESP_LOGI(TAG, "Partition size: total: %d, used: %d", total, used);
   }
}


void spiffs_handler_read()
{
   //Open renamed file for reading
   ESP_LOGI(TAG, "spiffs_handler_read() Reading file!");

   FILE* file = fopen("/spiffs/yoga_pose.ucf", "r");

   if(file != NULL)
   {
      ESP_LOGI(TAG, "spiffs_handler_read() Reading file successfully!");
      char line[200];
      int idx = 0;
      unsigned char address[1000];
      unsigned char data[1000];

      while(fgets(line, sizeof(line), file) != NULL)
      {
//         char* pos = strchr(line, '\n');
//         if(pos)
//         {
//            *pos = '\0';
//         }
//         ESP_LOGI(TAG, "spiffs_handler_read() Read from file: '%s'", line);

         if(('A' == line[0]) && ('c' == line[1]))
         {
            address[idx] = spiffs_handler_get_hex_value(line[3])*16 + spiffs_handler_get_hex_value(line[4]);
            data[idx] = spiffs_handler_get_hex_value(line[6])*16 + spiffs_handler_get_hex_value(line[7]);
            idx++;
         }
         bsp_delay_ms(10);
      }
      fclose(file);

      ESP_LOGW(TAG, "spiffs_handler_read() number of ucf element: %d", idx);

      ucf_line_t ucf_yoga_pose[idx];

      for(int i = 0; i <= idx; ++i)
      {
         ucf_yoga_pose[i].address = address[i];
         ucf_yoga_pose[i].data = data[i];
         ESP_LOGW(TAG, "ucf_yoga_pose-----%02X, %02X", ucf_yoga_pose[i].address, ucf_yoga_pose[i].data);
      }

      lsm6dsox_set_decision_tree(ucf_yoga_pose);
   }
   else
   {
      ESP_LOGE(TAG, "Failed to open file for reading");
   }
}

void spiffs_handler_deregister()
{
   // All done, unmount partition and disable SPIFFS
   esp_vfs_spiffs_unregister(conf.partition_label);
   ESP_LOGI(TAG, "SPIFFS unmounted");
}


unsigned char spiffs_handler_get_hex_value(const char letter)
{
   unsigned char hex_val = 0x00;

   if((letter - '0') <= 9)
   {
      hex_val = letter - '0';
   }
   else
   {
      hex_val = (letter - 'A') + 10;
   }

   return hex_val;
}
