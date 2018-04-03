#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>


static size_t read_ip_data_cb(char *, size_t , size_t , char *);

static size_t
read_ip_data_cb(char *data, size_t size, size_t nmemb, char *str) {
  snprintf(str, 99, "%s", data);

  return (nmemb * size);
}

int main(void) {
  char str[100] = "0";
  const char *const provider1 = "https://api.ipify.org";
  /* const char *const provider2 = "https://ipv4bot.whatismyipaddress.com"; */
  /* const char *const provider3 = "https://ident.me"; */
  /* const char *const provider4 = "https://wtfismyip.com/text"; */

  CURL *curl = NULL;
  CURLcode res;

  curl_global_init(CURL_GLOBAL_ALL);

  if (NULL == (curl = curl_easy_init())) {
    goto error;
  }

  curl_easy_setopt(curl, CURLOPT_URL, provider1);
  curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "gzip");
  curl_easy_setopt(curl, CURLOPT_USE_SSL, (long)CURLUSESSL_ALL); 
  curl_easy_setopt(curl, CURLOPT_TIMEOUT, 20L);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, read_ip_data_cb);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, str);

  res = curl_easy_perform(curl);
  if (CURLE_OK != res) {
    goto error;
  }

  printf("%s\n", str);

error:
  if (NULL != curl) {
    curl_easy_cleanup(curl);
  }
  curl_global_cleanup();
  return EXIT_SUCCESS;
}

