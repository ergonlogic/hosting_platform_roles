Hosting SaaS is a Drupal module intended to extend Aegir to enable more SaaS-
like functionality. 

The current plan
================

Required additions to the front-end:
 - add a checkbox to the platform form: "Enable SaaS on this platform"
 - add a "SaaS" tab to the platform page that includes (for each profile 
   available on the platform):
   - a field for an Aegir user that will be assigned UID1 for sites created
     using that profile
   - a field for the role name to assign site creators

When a site is created on a SaaS-enabled platform:
 - the current client will be saved as saas-client (in the site context)
 - the current client will be replaced (in the site context) by the Aegir user
   specified on the platform's SaaS tab for the relevant profile
 - after the site is installed, a second user will be created based on saas-
   client, and assigned the role specified on the platform's SaaS tab for the
   relevant profile
 - the "Login" link will be updated to point to the second user's account
 - Welcome email will be sent to the second user


