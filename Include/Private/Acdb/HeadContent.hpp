/*------------------------------------------------------------------------------
Copyright 2021 Garmin Ltd. or its subsidiaries.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
------------------------------------------------------------------------------*/

/**
    @file
    Style definition for rendering ACDB markers

    Copyright 2019-2021 by Garmin Ltd. or its subsidiaries.
*/

#ifndef ACDB_PRESENTATION_HeadContent_hpp
#define ACDB_PRESENTATION_HeadContent_hpp

static const char* ACDB_HEADCONTENT = R"(
<meta charset="utf-8">
<style>
    body {
        background-color: #ffffff;
        color: #000000;
        font-family:"Helvetica Neue",Helvetica,Arial,sans-serif;
        font-size:14px;
        margin:0
    }

    button.call_to_action {
        background-color:#337ab7;
        border:1px solid #122b40;
        border-radius:3px;
        color:#fff;
        cursor:pointer;
        padding:5px 10px
    }

    span.back_button {
        display:inline-block;
    }

    div.business_photos {
        min-width:350px;
        width:350px
    }

    div.competitor_ad {
        display:flex
    }

    div.competitor_ad_photo_container {
        display: flex;
        -webkit-box-pack: center;
        justify-content: center;
        -webkit-box-align: center;
        align-items: center;
        background: rgb(238, 238, 238);
        overflow: hidden;
        height: 90px;
        width: 160px;
    }

    div.content {
        padding:8px
    }

    div.list_page_header {
        display:inline-block;
        width:100%;
        box-sizing: border-box;
        padding: 10px;
        background-color: #337ab7;
        color: #fff;
    }

    div.list_page_content {
        padding-left:8px;
        padding-right:8px;
        padding-bottom:8px;
    }

    div.list_page_navigation {
        width: 100%;
    }

    div.list_title {
        display:inline-block;
        vertical-align:middle;
    }

    div.page_navigation {
        display: inline-block;
        font-size: 16px;
        padding-top: 8px;
        padding-bottom: 8px;
        text-align: center;
        width: 50%;
    }

    div.row {
        margin-top:14px;
    }

    div.review {
        margin-top:14px;
    }

    div.review_header {
        font-size:16px;
        margin-top:14px;
    }

    div.review_link {
        text-align:right
    }

    div.yes_no_compact_row {
        display: flex;
        align-items: center;
        margin-top: 8px;
        width: 100%;
    }

    div.yes_no_compact_column_left {
        display: flex;
        align-items: center;
        width: 50%;
        float: left;
    }

    div.yes_no_compact_column_right {
        display: flex;
        align-items: center;
        width: 50%;
        float: right;
    }

    div.yes_no_item {
        margin-top:14px;
    }

    h1 {
        font-size:24px;
        margin:0
    }

    h2 {
        font-size:18px;
        margin:0
    }

    h3 {
        font-size:14px;
        margin:0
    }

    hr {
        border:0;
        height:1px;
        background:#333;
        margin:10px 0 10px 0
    }

    hr.user_review {
        border:0;
        height:3px;
        background:#333;
        margin:10px 0 10px 0
    }

    img {
        vertical-align:middle
    }

    img.main_photo {
        min-height:150px;
        min-width:350px;
        max-height:600px;
        object-fit:cover;
        width:350px
    }

    img.business_photo {
        margin-top:8px;
        min-height:150px;
        min-width:350px;
        max-height:600px;
        object-fit:cover;
        width:350px
    }

    img.competitor_ad_photo {
        max-width: 10%;
        max-height: 10%;
        transform: scale(10);
    }

    img.yes_no_compact_icon {
        float: left;
    }

    span.bordered_text {
        border: 1px solid #0ba369;
        border-radius: 4px;
        color: #0ba369;
        float: right;
        padding: 1px 4px;
        position: relative;
        top: -1px;
    }

    span.edit_button {
        float: right;
    }

    span.review_button {
        float: right;
    }

    span.review_vote {
        float: right;
    }

    span.yes_no_compact_label {
        margin-left: 4px;
        word-break: normal;
        overflow-wrap: anywhere;
    }

    a {
        color:#337ab7;
        text-decoration:none;
        background-color:transparent
    }

    .inline {
        display:inline
    }

    .text_btn {
        background-color:#337ab7;
        color:#fff;
        display:inline-block;
        padding:6px 12px;
        margin-bottom:0;
        font-size:14px;
        line-height:1.42857143;
        text-align:center;
        white-space:nowrap;
        vertical-align:middle;
        border:1px solid transparent;
        border-color:transparent;
        border-radius:4px
    }

    div.review_photos {
        min-width:350px;
        width:350px;
        margin-top:8px;
    }

    img.review_photo {
        margin-top:8px;
        min-height:150px;
        min-width:350px;
        max-height:600px;
        object-fit:cover;
        width:350px;
    }

    .pre-wrap {
        white-space: pre-wrap;
    }
</style>
)";

#endif  // end of ACDB_PRESENTATION_HeadContent_hpp
