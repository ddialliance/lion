lion
====

Drupal site and custom modules for modeling DDI in the project [DDI Moving Forward](https://ddi-alliance.atlassian.net/wiki/pages/viewpage.action?pageId=491703)

# Setup

1. Install [docker](https://www.docker.com)

2. Clone this repository

3. Get SQL-dump from a friendly developer and place it in `sql-dum`

5. Copy `settings.example.php` to a file named `settings.php`.
   default username password can be used during local development (no changes needed), change it if you want to use this setup for a public server.

5. Run `docker-compose up` this will import the sql-dump and set up the system

6. Your local development instance should now run on http://localhost:8080

# Development terminal

To get an interactive shell for the container run: `docker exec -it lion_web_1 /bin/bash`

In the terminal you can use [drush](https://drushcommands.com) to do various tasks.

Some examples:

* `drush cc all` clear all cache
* `drush sql-query 'DELETE FROM cache;'` force clear all cache by deleting everything in the cache table
* `drush ws` show latest log entries

# Overview

Custom modules include: 
## [xmi_export](https://github.com/ddialliance/lion/tree/master/modules/custom/xmi_export)
Used to get data from drupal and export it as XMI-XML.

* [main entry point for menu routes](https://github.com/ddialliance/lion/blob/master/modules/custom/xmi_export/xmi_export.module#L6) defines the paths fo activate each function


## [ddi_model_import](https://github.com/ddialliance/lion/tree/master/modules/custom/ddi_model_import)
Used to do the first import of DDI-L 3.x import of object. Did only run once in the early stages of DDI moving forward.
