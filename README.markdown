Hosting SaaS is a Drupal module intended to extend Aegir to enable more SaaS-
like functionality. 

The current plan
============

Required additions to the front-end:

- add a "SaaS" tab to the platform page that includes (for each profile available on the platform): **[done]**
    - a field for an Aegir user that will be assigned UID1 for sites created
     using that profile **[done]**
    - a field for the role name to assign site creators **[done]**

When a site is created on a SaaS-enabled platform:

- UID1 on a created site is replaced (in the site context) by the Aegir user specified on the platform's SaaS tab for the relevant profile **[done]**
- after the site is installed, a second user will be created based on site's creator **[done]**
- that user is assigned the role specified on the platform's SaaS tab for the relevant profile **[done]**
- the "Login" link will be updated to point to the second user's account
- Welcome email will be sent to the second user

Questions
=======
- Should the SaaS setting for the platform be saved into the platform context?


Notes
====
- drush user-login [username]

