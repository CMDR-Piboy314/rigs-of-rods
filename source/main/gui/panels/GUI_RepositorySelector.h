/*
    This source file is part of Rigs of Rods
    Copyright 2005-2012 Pierre-Michel Ricordel
    Copyright 2007-2012 Thomas Fischer
    Copyright 2013-2021 Petr Ohlidal

    For more information, see http://www.rigsofrods.org/

    Rigs of Rods is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 3, as
    published by the Free Software Foundation.

    Rigs of Rods is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Rigs of Rods. If not, see <http://www.gnu.org/licenses/>.
*/

/// @file   GUI_RepositorySelector.h
/// @author Rafael Galvan, 04/2021
/// @author Petr Ohlidal, 2022
/// @author tritonas00, 2022

#pragma once

#include "Application.h"
#include "OgreImGui.h" // ImVec4

#include <future>
#include <memory>
#include <thread>
#include <vector>
#ifdef USE_CURL
#   include <curl/curl.h>
#endif //USE_CURL

namespace RoR {
namespace GUI {

struct ResourceCategories
{
    int                 resource_category_id;
    std::string         title;
    std::string         description;
    int                 resource_count;
    int                 display_order;
};

struct ResourceItem
{
    int                 resource_id;
    std::string         title;
    std::string         tag_line;
    std::string         icon_url;
    std::string         authors;
    std::string         version;
    int                 download_count;
    int                 last_update;
    int                 resource_category_id;
    float               rating_avg;
    int                 rating_count;
    std::string         view_url;
    int                 resource_date;
    int                 view_count;
    Ogre::TexturePtr    preview_tex;
    bool                thumbnail_dl_queued = false;
};

struct ResourceFiles
{
    int                 id;
    std::string         filename;
    int                 size;
};

struct ResourcesCollection
{
    std::vector<ResourceItem>           items;
    std::vector<ResourceCategories>     categories;
    std::vector<ResourceFiles>          files;
};

class RepositorySelector:
    public Ogre::WorkQueue::RequestHandler, // Processes tasks on background thread
    public Ogre::WorkQueue::ResponseHandler // Processes task results on rendering thread
{
public:
    const Ogre::uint16                  WORKQUEUE_ROR_REPO_THUMBNAIL = 1; // Work queue request type, named by OGRE convention.

    RepositorySelector();
    ~RepositorySelector();

    void                                SetVisible(bool visible);
    bool                                IsVisible() const { return m_is_visible; }
    void                                Draw();
    void                                OpenResource(int resource_id);
    void                                Download(int resource_id, std::string filename, int id);
    void                                DownloadFinished();
    void                                Refresh();
    void                                UpdateResources(ResourcesCollection* data);
    void                                UpdateFiles(ResourcesCollection* data);
    void                                ShowError(std::string const& msg);
    void                                DrawThumbnail(int resource_item_idx);

    /// Ogre::WorkQueue API
    virtual Ogre::WorkQueue::Response*  handleRequest(const Ogre::WorkQueue::Request *req, const Ogre::WorkQueue *srcQ) override; //!< Processes tasks on background thread
    virtual void                        handleResponse(const Ogre::WorkQueue::Response *req, const Ogre::WorkQueue *srcQ) override; //!< Processes task results on main thread

private:
    bool                                m_is_visible = false;
    bool                                m_draw = false;
    std::string                         m_repolist_msg;
    ImVec4                              m_repolist_msg_color;
    std::string                         m_repofiles_msg;
    ResourcesCollection                 m_data;
    Str<500>                            m_search_input;
    std::string                         m_current_category;
    int                                 m_current_category_id = 1;
    std::string                         m_all_category_label;
    std::string                         m_current_category_label;
    bool                                m_update_cache = false;
    bool                                m_show_spinner = false;
    std::string                         m_current_sort = "Last Update";
    std::string                         m_view_mode = "List";
    bool                                m_resource_view = false;
    ResourceItem                        m_selected_item;
    Ogre::uint16                        m_ogre_workqueue_channel = 0;
    Ogre::TexturePtr                    m_fallback_thumbnail;
#ifdef USE_CURL
    CURL                                *curl_th = curl_easy_init(); // One connection for fetching thumbnails using connection reuse
#endif
};

}// namespace GUI
}// namespace RoR
