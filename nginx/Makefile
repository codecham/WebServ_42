CONF_PATH = /Users/corentin/Desktop/19/webserv_local/nginx/nginx.conf

all: 
	sudo nginx -c $(CONF_PATH)

stop: 
	sudo nginx -s stop

re: 
	sudo nginx -s reload

update:
	sudo rm -rf /var/www/website/*
	sudo cp -R website/* /var/www/website/
	sudo chmod -R 777 /var/www/website
	