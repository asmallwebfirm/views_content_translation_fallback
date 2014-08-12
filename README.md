Views Content Translation Fallback
===================================

This Drupal extension provides a simple Views filter that handles complex
content negotiation rules for you. It can be very useful if you use the Content
Translation module on Drupal sites with content in various states of translation
completeness.


### Installation and usage

This module assumes you already have the Views and Content Translation modules
installed and enabled. You can enable this module like you enable any other
Drupal module.

Once installed, go to an existing content View, or create a new content View,
and click "Add" in the filter criteria section. This will open the "Add filter
criteria" dialogue.

In the dialogue, search for the "Content translation: Fallback" filter, check
it, then press "Add and configure filter criteria."

You can optionally configure the "fallback language" for this filter; it will
default to the site's default language.


### Background

What does this filter do, exactly? Before we answer that, a little background!

Drupal Core provides a way to translate content using a paradigm called "Content
Translation" (used to differentiate an alternate translation paradigm known as
[Entity Field Translation](https://drupal.org/project/entity_translation)).

The _content translation_ paradigm works by storing translations of content in
distinct nodes that are conceptually linked together in a "translation set." On
any given node, you can find the "master" node, from which other nodes have been
translated, by checking against the node's `tnid` property (representing the
`nid` of the master node in the translation set).

Using this paradigm, it becomes necessary to apply a language filter to every
content View, indicating that only content in the current site's language should
be returned and displayed.

On site's that allow content to be in a "language neutral" state, it's necessary
to make the aforementioned language filter include "language neutral" in
addition to the "current site language" filter criteria.

The problem with this technique is that it simply doesn't show content from a
given translation set if there's no content in the set that matches the current
site language (e.g. an English piece of content is translated into French, but
the View is being loaded on the Spanish site).

In many cases, it's desirable to "fall back" to a default language, but setting
up a filter to manage this can be quite cumbersome, and is not very easy to
scale across all Views across your site.


### How it works

The way to work around this problem is somewhat complex:

* You must add a relationship to your View, `LEFT JOIN`ing the node table onto
  itself using the tnid.
* You must `OR` your usual current language and language neutral content
  language filter with another language filter, checking for your fallback
  language, and `AND`ing it with a check to make sure the node ID of the joined
  node table is empty.

Due to the nature of condition groups in Views, this can be extremely cumbersome
to manage, especially on more complex Views.

This module solves the problem by packaging the above relationship/filter group
combination into a single filter criterion that you can add to any view. No
Views relationships or filter group knowledge necessary.
