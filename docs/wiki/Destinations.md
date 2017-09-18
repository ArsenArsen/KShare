---
title: Destinations
categories: [wiki]
layout: wikipage
---
# Destinations
Destinations determine where your image goes

There is a set of default uploaders, two, to be exact.
## Default uploaders:
### imgur
Uploads your image to imgur.
### clipboard
Copies your image to clipboard.

## Custom uploaders
Placed into `CONFIG PATH/KShare/uploaders`  
Eg: `$HOME/.config/KShare/uploaders`  
All uploaders have to be `.uploader` files!  
Custom uploaders can be added, and are made with JSON, and follow this template:
```js
{
 "name": "", // String, required
 "desc": "", // String, optional
 "method": "", // String, default: "POST", for now, just POST. Request more if you need them
 "target": "", // URL, required
 "format": "", // String, default: "json", can be `x-www-form-urlencoded` or `json` or `multipart-form-data`. Setting to `PLAIN` will make the body ignored and only the image sent.
 "base64": false, // Optional, makes the data base64
 "body": {}, // A JSON object, where one of the nodes can be a string in the format `/ANYTHING/`. In `ANYTHING`, `%contenttype` is replaced with the image type and `%imagedata` is replaced with the image encoded with `imageformat`. Unless you use multipart, see below
 "return": "" // Return pathspec. `|` copies the entire body, `.path.to.node` copies the value of the node. The dot IS IMPORTANT. Without it, nothing happens. If one of the nodes is not a string, the string is copied, if one is an object, recursion continues, if one is null, nothing is copied, otherwise the node is JSON stringified. Only supports JSON for now. 
}
```
Note that QJson does not support comments.
### Multipart
Multipart is obviously made out of multiple parts. The way you define a multipart body is:
```js
[
    {
        "__HeaderName": "HeaderValue", // No limit here. Must start with __, which is removed later.
        "body": { /* same way you define it for anything json */ }, // Can be string. Strings matching `/.../` are processed same way as in a json field.
        "name": "files[]" // Info to add to Content-Disposition (eg part name, filename, ...)
    }
]
```
This would go in the `body` field of the above JSON.
