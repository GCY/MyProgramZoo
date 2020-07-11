#include "esp_camera.h"
#include "SSD1306.h"

SSD1306 display(0x3C,15,13);

#define SCREEN_WIDTH 128 // OLED显示宽度，以像素为单位
#define SCREEN_HEIGHT 64 


#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

static camera_config_t camera_config = {
  .pin_pwdn = PWDN_GPIO_NUM,
  .pin_reset = RESET_GPIO_NUM,
  .pin_xclk = XCLK_GPIO_NUM, 
  .pin_sscb_sda = SIOD_GPIO_NUM,  
  .pin_sscb_scl = SIOC_GPIO_NUM,
     
  .pin_d7 = Y9_GPIO_NUM,
  .pin_d6 = Y8_GPIO_NUM,
  .pin_d5 = Y7_GPIO_NUM, 
  .pin_d4 = Y6_GPIO_NUM,
  .pin_d3 = Y5_GPIO_NUM,
  .pin_d2 = Y4_GPIO_NUM,  
  .pin_d1 = Y3_GPIO_NUM,
  .pin_d0 = Y2_GPIO_NUM,
  .pin_vsync = VSYNC_GPIO_NUM,
  .pin_href = HREF_GPIO_NUM,
  .pin_pclk = PCLK_GPIO_NUM,

  .xclk_freq_hz = 20000000,
  .ledc_timer = LEDC_TIMER_0,
  .ledc_channel = LEDC_CHANNEL_0,
  
  .pixel_format = PIXFORMAT_GRAYSCALE,
  .frame_size = FRAMESIZE_QQVGA2,
  .jpeg_quality = 12,
  .fb_count = 1,
};

byte x,y;
uint8_t oldpixel, newpixel;
int quanterror;

esp_err_t camera_init(){
    //initialize the camera
    esp_err_t err = esp_camera_init(&camera_config);
    if (err != ESP_OK) {
        Serial.print("Camera Init Failed");
        return err;
    }
    sensor_t * s = esp_camera_sensor_get();
    //initial sensors are flipped vertically and colors are a bit saturated
    if (s->id.PID == OV2640_PID) {
        s->set_vflip(s, 1);//flip it back
        s->set_brightness(s, 1);//up the blightness just a bit
        s->set_contrast(s, 1);
    }
  
    Serial.print("Camera Init OK");
    return ESP_OK;
}

void my_camera_show(void)
{
    camera_fb_t *fb = esp_camera_fb_get();
    int i=0;
    for(y = 0; y <SCREEN_HEIGHT; y ++)
    {
      for(x = 0; x <SCREEN_WIDTH; x ++)
      {
        oldpixel = fb-> buf [i]; //保持原始的灰度值
        newpixel =(255 *(oldpixel >> 7)); //门槛值128
        fb-> buf [i] = newpixel; //凸轮缓冲区现在是单声道，0或255
    
        // floyd-steignburg抖动：
        quanterror = oldpixel  -  newpixel; //像素之间的误差
    
        //将此错误分发给相邻像素：
    
       //右
          if(x <SCREEN_WIDTH-1)//边界检查...
          {
    
            fb-> buf [(x + 1)+(y * SCREEN_WIDTH)] +=((quanterror * 7)>> 4);
          }
    
          // 左下
          if((x> 1)&&(y <SCREEN_HEIGHT-1))//边界检查...
          {
            fb-> buf [(x-1)+((y + 1)* SCREEN_WIDTH)] ==((quanterror * 3)>> 4); 
          }
    
          //下
          if(y <63)//边界检查...
          {
             fb-> buf [(x)+((y + 1)* SCREEN_WIDTH)] ==((quanterror * 5)>> 4);
          }
    
          // 右下
          if((x <SCREEN_WIDTH-1)&&(y <SCREEN_HEIGHT-1))//边界检查...
          {
             fb-> buf [(x + 1)+((y + 1)* SCREEN_WIDTH)] ==(quanterror >> 4);
          }

        // 画这个像素

        switch(fb->buf[i]%2)
        {
          case 0:
            display.setColor(BLACK);
            break;
          case 1:
            display.setColor(WHITE);
            break;
          case 2:
            display.setColor(INVERSE);
            break;
        }
        display.setPixel(x, y);
        i++;
      }
    }
    display.display();
    esp_camera_fb_return(fb);
}


void setup() {
  Serial.begin(115200);
  display.init();
  camera_init();
  
  Serial.setDebugOutput(true);
  Serial.println();
  Serial.print("sys is 1...");
  Serial.print("sys is running!");
}

void loop() {
   my_camera_show();
}

