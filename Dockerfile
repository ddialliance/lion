FROM drupal:7.50-apache
MAINTAINER olof olsson <olof.olsson@snd.gu.se>

RUN apt-get update &&\
	apt-get install -y mysql-client &&\
	apt-get install -y git


# Register the COMPOSER_HOME environment variable
ENV COMPOSER_HOME /composer

# Add global binary directory to PATH and make sure to re-export it
ENV PATH /composer/vendor/bin:$PATH
ENV COMPOSER_VERSION 1.1.2

# Install Composer and drush
ENV COMPOSER_INSTALLER_SHA384 "aa96f26c2b67226a324c27919f1eb05f21c248b987e6195cad9690d5c1ff713d53020a02ac8c217dbf90a7eacc9d141d"
RUN php -r "readfile('https://getcomposer.org/installer');" > /tmp/composer-setup.php &&\
	php -r "if (hash('SHA384', file_get_contents('/tmp/composer-setup.php')) !== '${COMPOSER_INSTALLER_SHA384}') { unlink('/tmp/composer-setup.php'); echo 'Invalid installer' . PHP_EOL; exit(1); }" &&\
	export PATH="$HOME/.composer/vendor/bin:$PATH" &&\
	php /tmp/composer-setup.php --no-ansi --install-dir=/usr/local/bin --filename=composer --version=${COMPOSER_VERSION} && rm -rf /tmp/composer-setup.php &&\
	composer --version  &&\
	composer global require drush/drush:8.* &&\
	drush @none dl registry_rebuild-7.x &&\
	drush cc drush

RUN echo 'alias init-site="drush updb -y && drush rr && drush composer-json-rebuild && drush composer-manager install -y"' >> ~/.bashrc

WORKDIR /var/www/html/sites/default	

VOLUME ["/src"]