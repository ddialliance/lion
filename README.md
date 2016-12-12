Lion -Capturing platform for the DDI4 model
===========================================

Drupal 7 site and custom modules for modeling DDI4 in the project [DDI Moving Forward](https://ddi-alliance.atlassian.net/wiki/pages/viewpage.action?pageId=491703)

# Setup

1. Install [docker](https://www.docker.com)

2. Clone this repository

3. Get SQL-dump from a friendly developer and place it in `sql-dump`

5. Copy `settings.example.php` to a file named `settings.php`.
   default username password can be used during local development (no changes needed), change it if you want to use this setup for a public server.

5. Run `docker-compose up` this will import the sql-dump and set up the system

6. Your local development instance should now run on http://localhost:8080

# Development terminal

To get an interactive shell for the container run: `docker exec -it lion_web_1 /bin/bash` (if your continer is named `lion_web_1`)

In the terminal you can use [drush](https://drushcommands.com) to do various tasks.

Some examples:

* `drush cc all` clear all cache
* `drush sql-query 'DELETE FROM cache;'` force clear all cache by deleting everything in the cache table
* `drush ws` show latest log entries

# Overview

Custom modules include: 
## [xmi_export](https://github.com/ddialliance/lion/tree/master/modules/custom/xmi_export)
Used to get data from drupal and export it as XMI-XML.

Functional overview:

* [main entry point for menu routes](https://github.com/ddialliance/lion/blob/master/modules/custom/xmi_export/xmi_export.module#L6) defines the paths fo activate each function
* [handle read from the database](https://github.com/ddialliance/lion/blob/master/modules/custom/xmi_export/xmi_export.inc)
* [page callback for xmi file generation](https://github.com/ddialliance/lion/blob/master/modules/custom/xmi_export/xmi_export.inc#L215)
* [validation checks of naming conventions and links to exluded packages](https://github.com/ddialliance/lion/blob/master/modules/custom/xmi_export/validate.inc)
* [page callbakc for mappings overview DDI-L / GSIM](https://github.com/ddialliance/lion/blob/master/modules/custom/xmi_export/mapping.inc)

## [ddi_model_import](https://github.com/ddialliance/lion/tree/master/modules/custom/ddi_model_import)
Used to do the first import of DDI-L 3.x import of object. Did only run once in the early stages of DDI moving forward.

# License
[MIT](LICENSE.md)