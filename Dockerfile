FROM drupal:7.56-apache
MAINTAINER SND <webmaster@snd.gu.se>

RUN apt-get update &&\
	apt-get install -y mysql-client &&\
	apt-get install -y git

RUN echo "short_open_tag = Off" > /usr/local/etc/php/conf.d/short_tags_off.ini

# Register the COMPOSER_HOME environment variable
ENV COMPOSER_HOME /composer

# Add global binary directory to PATH and make sure to re-export it
ENV PATH /composer/vendor/bin:$PATH
ENV COMPOSER_VERSION 1.1.2

# Install Composer and drush
ENV COMPOSER_INSTALLER_SHA384 "544e09ee996cdf60ece3804abc52599c22b1f40f4323403c44d44fdfdd586475ca9813a858088ffbc1f233e9b180f061" 
RUN php -r "readfile('https://getcomposer.org/installer');" > /tmp/composer-setup.php &&\
	php -r "if (hash('SHA384', file_get_contents('/tmp/composer-setup.php')) !== '${COMPOSER_INSTALLER_SHA384}') { unlink('/tmp/composer-setup.php'); echo 'Invalid installer' . PHP_EOL; exit(1); }" &&\
	export PATH="$HOME/.composer/vendor/bin:$PATH" &&\
	php /tmp/composer-setup.php --no-ansi --install-dir=/usr/local/bin --filename=composer --version=${COMPOSER_VERSION} && rm -rf /tmp/composer-setup.php &&\
	composer --version  &&\
	composer global require drush/drush:8.* &&\
	composer global require squizlabs/php_codesniffer:2.7.0 &&\
	composer global require drupal/coder &&\
	phpcs --config-set installed_paths /composer/vendor/drupal/coder/coder_sniffer &&\
	phpcs --config-set default_standard Drupal &&\
	drush @none dl registry_rebuild-7.x &&\
	drush cc drush

WORKDIR /var/www/html/sites/default	

VOLUME ["/src"]