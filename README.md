lion
====

Drupal site and custom modules for modeling DDI

# Setup

1. Install [docker](https://www.docker.com)

2. Clone this repository

3. Get SQL-dump from a friendly developer and place it in `sql-dum`

5. Copy `settings.example.php` to a file named `settings.php`.
   default username password can be used during local development (no changes needed), change it if you want to use this setup for a public server.

5. Run `docker-compose up` this will import the sql-dump and set up the system

6. Your local development instance should now run on http://localhost:8080

# Overview

Custom modules include: 
## [xmi_export](https://github.com/ddialliance/lion/tree/master/modules/custom/xmi_export)
Used to get data from drupal and export it as XMI-XML.


## [ddi_model_import](https://github.com/ddialliance/lion/tree/master/modules/custom/ddi_model_import)

