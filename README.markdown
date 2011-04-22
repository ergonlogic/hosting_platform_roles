Hosting Platform Roles
======================

Hosting Platform Roles is a Drupal module that extends Aegir to enable more
control over what role(s) a client is assigned during site install. Since, by
default, Aegir client's are given UID1 during site installs, this module also
allows UID1 to be assigned to another user. This allows tighter control of the
client's user experience, and thus support for Software-as-a-Service (SaaS)
type business models.

Current features
================

Additions to the front-end:

- adds a "Roles" tab to the platform page that includes (for each profile
  available on the platform):
    - a field for an Aegir user that will be assigned UID1 for sites created
     using that profile
    - a field for the name of the role to assign site creators

When a site is created on a such a platform:

- UID1 on a created site is replaced (in the site context) by the Aegir user
  specified on the platform's Roles tab for the relevant profile
- after the site is installed, a second user is created based on site's creator
- that user is assigned the role specified on the platform's Roles tab for the
  relevant profile
- the "Login" link is updated to point to the second user's account

To do
=====

- comment ALL functions **[in progress]**
- remove hook_forms() and hook_page() **[done]**
- add .install to create/drop db tables **[done]**
- allow for an aegir-wide default for uid1 (?)
- move from using variables table to our own db tables **[done]**
- call drush commands individually **[done]**
- only set the login link when uid1 is set for a platform **[done]**
- validate form input **[in progress]**
- remove un-necessary variables **[mostly done]**
- re-write comments to explains the whys, rather than the hows
- add approproate t()'s **[in progress]**
- call hosting_get_profiles() in the form_submit **[done]**
- write drush email code, rather than depend on a patch to drush **[done]**
- allow for multiple roles to be assigned

