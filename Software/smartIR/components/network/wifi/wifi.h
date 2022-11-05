#ifndef _WIFI_H_
#define _WIFI_H_

// 需要先init nvs
void wifi_init_sta(void);

// 包括nvs的init
void wifi_init(void);
void wifi_start(void);
void wifi_stop(void);


#endif
